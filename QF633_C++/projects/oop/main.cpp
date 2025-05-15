#include <fstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <utility>
#include <string>

#include "Market.h"
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "AmericanTrade.h"

using namespace std;

vector<pair<string, string>> readKeyValueFile(const string& fileName) {
    vector<pair<string, string>> result;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos == string::npos) continue;
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        // Trim leading/trailing whitespace
        key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);

        // Remove % and \r
        value.erase(remove(value.begin(), value.end(), '%'), value.end());
        value.erase(remove(value.begin(), value.end(), '\r'), value.end());

        result.emplace_back(key, value);
    }
    return result;
}

int main()
{
	//task 1, create an market data object, and update the market data from from txt file
	Date valueDate;
	time_t t = time(nullptr);
	struct tm timeInfo;
	if (localtime_r(&t, &timeInfo) != nullptr) {
		// localtime_s() convert current system time into localtime and populate tm struct
		valueDate.year = timeInfo.tm_year + 1900; //1900 based
		valueDate.month = timeInfo.tm_mon + 1; //0 based
		valueDate.day = timeInfo.tm_mday;
	};
	cout << valueDate << endl;			// print out the current date
	//Date newDate;
	//cin >> newDate;
	//cout << newDate;
	

	Market mkt;

	/*
	load data from file and update market object with data
	*/
	string curveFile = "curve.txt";
	string volFile = "vol.txt";	
	string bondFile = "bondPrice.txt";
	string stockFile = "stockPrice.txt";
	string curveData;
	string volData;	
	string bondData;
	string stockData;

	auto curvePairs = readKeyValueFile(curveFile);
	auto volPairs = readKeyValueFile(volFile);
	auto bondPairs = readKeyValueFile(bondFile);
	auto stockPairs = readKeyValueFile(stockFile);

	// readFromFile(volFile, volData);
	// readFromFile(bondFile, bondData);
	// readFromFile(stockFile, stockData);

	RateCurve usdSofr("USD-SOFR");
	// ifstream curveStream(curveFile);

	//task 2, create a portfolio of bond, swap, european option, american option
	//for each time, at least should have long / short, different tenor or expiry, different underlying
	//totally no less than 16 trades
	vector<Trade*> myPortfolio;										// deep copy constructor
	Trade* bond = new Bond("tesst", Date(2023, 12, 31), Date(2024, 1, 1), Date(2034, 1, 1), 10000000, 2, 103.5);
	myPortfolio.push_back(bond);	


	//task 3, creat a pricer and price the portfolio, output the pricing result of each deal.
	Pricer* treePricer = new CRRBinomialTreePricer(10);
	for (auto trade : myPortfolio) {
		double pv = treePricer->Price(mkt, trade);
		//log pv details out in a file

	}

	//task 4, analyzing pricing result
	// a) compare CRR binomial tree result for an european option vs Black model
	// b) compare CRR binomial tree result for an american option call vs european option call, and put

	//final
	cout << "Project build successfully!" << endl;
	return 0;

}
