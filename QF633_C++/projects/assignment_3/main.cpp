#include <iostream>
#include<fstream>
#include <vector>
#include <stdexcept>
#include <string>

#include "black.h"

using namespace std;

// Splits a string into a vector of substrings based on a given separator.
void splitString(vector<string>&output, const string& inputLine, const char separator)
{
    output.clear();
    size_t start = 0;
    size_t end = inputLine.find(separator);

    while (end != std::string::npos) {
        output.push_back(inputLine.substr(start, end - start)); // Extract substring
        start = end + 1; // Move past the separator
        end = inputLine.find(separator, start); // Find next separator
    }
    output.push_back(inputLine.substr(start)); // Add last part of the string
}

void loadTradeFromFile(vector<OptionTrade>&tradesSet, const string& filePath) 
{
   std::ifstream inputFile(filePath); // Open the file
   try {
       if (inputFile) {
           string line;
           if (getline(inputFile, line)) {
               cout << "Skipped first row: " << line << endl; // Optional: Print skipped line
           }

           while (getline(inputFile, line)) { // Read each line
               cout << line << endl;    // Print the line (or process it)
               vector<string> lineOfTrade;
               splitString(lineOfTrade, line, ';');
               int tradeId = stoi(lineOfTrade[0]);
               if (tradeId < 6) {
                   OptionTrade ot;
                   ot.expiry = stod(lineOfTrade[4]);
                   ot.isCall = lineOfTrade[3] == "true" ? true : false;
                   try {
                       for (char c : lineOfTrade[1]) {
                           if (!isdigit(c) && c != '.') {
                               throw invalid_argument("Invalid notional value: contains non-numeric characters");
                           }
                       }
                       ot.notional = stod(lineOfTrade[1]);
                   } catch (const invalid_argument& e) {
                       cerr << "Error processing notional value: " << e.what() << " in line: " << line << endl;
                       continue; // Skip this trade and move to the next
                   }
                   ot.strike = stod(lineOfTrade[2]);
                   tradesSet.push_back(ot);
               }
           }
           inputFile.close(); // Close the file

       }
       else
           cout << "file does not exsits" << endl;
   } catch (const exception& e) {
       cerr << e.what() << endl;
   }
}

// save results
void writeResultToFile(const vector<double>& result, const string& fileName)
{
    ofstream outputFile(fileName); // Create or open the file for writing
    if (!outputFile) {
        cerr << "Error: Could not create or open file!" << endl;
    }
    else {
        for (auto& re : result) {
            outputFile << "trade pv: " << re <<endl;
        }        
    }
    outputFile.close();
    cout << "Data written to file successfully!" << std::endl;
}


int main () {
    cout << "compute option pv task is started." << endl;
    vector<OptionTrade> tradesSet;
    string file = "trades.txt";
    loadTradeFromFile(tradesSet, file);
    vector<double> pvResult;
    double spot = 100;
    double vol = .2;
    double rate = 0.045;

    for (auto& trade : tradesSet)
    {
        /*
        call black sholes model here
        * 
        */
        double n = trade.notional;        
        double pv = BlackScholes(trade.notional, trade.strike , trade.expiry, spot, vol, rate, trade.isCall);
        pvResult.push_back(pv);
    }

    /*
    save result back into a file
    */
    writeResultToFile(pvResult, "result.txt");
    cout << "compute option pv task is completed." << endl;
    return 0;
}