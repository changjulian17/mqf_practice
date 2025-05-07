#include <iostream>
#include<fstream>
#include <vector>
#include <iomanip> // for std::setprecision

#include "black.h"
#include "trade.h"

using namespace std;

struct PvResult {
    string trade_id;
    string trade_info;
    double pv;
};


#include <vector>
#include <string>

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

void loadTradeFromFile(vector<OptionTrade*>&tradesSet, const string& filename)
{
    /*
    load trade data from file
    insert into vector
    */

    // Open the file for reading
    ifstream input_file(filename);
    vector<string> lines;

    // Check if the file was opened successfully
    if (!input_file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
    }

    std::string line;
    // Read the file line by line
    while (std::getline(input_file, line)) {
        // Add each line to our vector
        lines.push_back(line);

        // You could also process each line immediately here
        // use split function to get the neede information for trade
        trade* optTrade = new OptionTrade();  // we want the base class pointer but need to create option trade object
        tradesSet.push_back(optTrade);
    }

    
}

void writeTofile(vector<PvResult>& result, const string& filename)
{
    /*
    load trade data from file
    insert into vector
    */

    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error opening file for writing!" << endl;
    }

    // Write header line
    outfile << left << setw(20) << "trade id" << setw(10) << "trade info" << setw(10) << "trade pv" << "\n";

    // Write separator line
    outfile << string(40, '-') << "\n";

    // Write each struct in a formatted way
    for (const auto& re : result) {
        outfile << left << std::setw(20) << re.trade_id 
            << right << std::setw(5) << re.trade_info 
            << setw(8) << std::fixed << std::setprecision(2) << re.pv << "\n";
    }
}

int main () {
    cout << "compute option pv task is started." << endl;
    vector<OptionTrade*> tradesSet; // base class pointer but need to create option trade object
    string file = "";
    loadTradeFromFile(tradesSet, file);
    vector<double> pvResult;
    double spot = 100;
    double vol = .2;
    double rate = 0.045;

    for (auto& trade : tradesSet)
    {

      
        double pv = trade.calculatePv(spot, vol, rate);
        pvResult.push_back(pv);
        delete trade; // free memory

    }


    tradeSet.clear(); // free memory
    /*
    save result back into a file
    */

    cout << "compute option pv task is completed." << endl;
    return 0;

}