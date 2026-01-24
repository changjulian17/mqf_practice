#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <regex>

#include "trade.h" // Include the OptionTrade class
#include "date.h"  // Include the Date class
#include "black.h"

using namespace std;

struct Result {
    int tradeId;
    double pv;
};

// Splits a string into a vector of substrings based on a given separator.
void splitString(vector<string>& output, const string& inputLine, const char separator) {
    output.clear();
    size_t start = 0;
    size_t end = inputLine.find(separator);

    while (end != string::npos) {
        output.push_back(inputLine.substr(start, end - start)); // Extract substring
        start = end + 1; // Move past the separator
        end = inputLine.find(separator, start); // Find next separator
    }
    output.push_back(inputLine.substr(start)); // Add last part of the string
}

// Load trades from file into a vector of OptionTrade objects
void loadTradeFromFile(vector<OptionTrade*>& tradesSet, const string& filePath) {
    ifstream inputFile(filePath); // Open the file
    try {
        if (inputFile) {
            string line;
            if (getline(inputFile, line)) {
                cout << "Skipped first row: " << line << endl; // Optional: Print skipped line
            }

            while (getline(inputFile, line)) { // Read each line
                vector<string> lineOfTrade;
                splitString(lineOfTrade, line, ';');
                int tradeId = stoi(lineOfTrade[0]);
                if (lineOfTrade.size() < 8) {
                    try {
                        if (!regex_match(lineOfTrade[1], regex(R"(^[-+]?\d*\.?\d+$)"))) {
                            cerr << "Invalid notional value: " << lineOfTrade[1] << " in line: " << line << endl;
                            continue; // Skip this trade and move to the next
                        }
                        double notional = stod(lineOfTrade[1]);
                        double strike = stod(lineOfTrade[2]);
                        bool isCall = lineOfTrade[3] == "true";
                        Date startDate(lineOfTrade[4].substr(1, lineOfTrade[4].size() - 2)); // Remove quotes
                        Date endDate(lineOfTrade[5].substr(1, lineOfTrade[5].size() - 2));   // Remove quotes

                        // Dynamically create an OptionTrade object with tradeId
                        OptionTrade* trade = new OptionTrade(notional, strike, startDate, endDate, isCall, tradeId);
                        tradesSet.push_back(trade);
                    } catch (const exception& e) {
                        cerr << "Error processing trade: " << e.what() << " in line: " << line << endl;
                        continue; // Skip this trade and move to the next
                    }
                }
            }
            inputFile.close(); // Close the file
        } else {
            cout << "File does not exist" << endl;
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
}

// Save results to a file
void writeResultToFile(const vector<Result>& results, const string& fileName) {
    ofstream outputFile(fileName); // Create or open the file for writing
    if (!outputFile) {
        cerr << "Error: Could not create or open file!" << endl;
    } else {
        for (const auto& result : results) {
            outputFile << "Trade ID: " << result.tradeId << ", PV: " << result.pv << endl;
        }
    }
    outputFile.close();
    cout << "Data written to file successfully!" << endl;
}

int main() {
    cout << "Compute option PV task is started." << endl;
    vector<OptionTrade*> tradesSet;
    string file = "trades.txt";
    loadTradeFromFile(tradesSet, file);

    vector<Result> results;
    double spot = 100;
    double vol = 0.2;
    double rate = 0.045;

    for (auto& trade : tradesSet) {
        int tradeId = trade->getTradeId(); // Get the trade ID from the OptionTrade object
        double pv = trade->calculatePv(spot, vol, rate);
        results.push_back({tradeId, pv}); // Add trade ID and PV to results
    }

    // Save result back into a file
    writeResultToFile(results, "result.txt");
    cout << "Compute option PV task is completed." << endl;

    // Clean up dynamically allocated memory
    for (auto& trade : tradesSet) {
        delete trade;
    }

    return 0;
}