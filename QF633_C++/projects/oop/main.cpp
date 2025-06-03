#include <fstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <utility>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <sstream>

#include "Market.h"
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "AmericanTrade.h"

using namespace std;

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
	
	/*
	load data from file and update market object with data
	*/
	int treeTimeSteps = 50;
	string curveFile = "curve.txt";
	string volFile = "vol.txt";	
	string bondFile = "bondPrice.txt";
	string stockFile = "stockPrice.txt";

	Market mkt = buildMarket(valueDate, curveFile, volFile, bondFile, stockFile);

    mkt.Print(); // print out the market data

    // Parse trade.txt and build portfolio
    vector<Trade*> myPortfolio;
    ifstream tradeFile("trade.txt");
    string line;
    getline(tradeFile, line); // skip header

    while (getline(tradeFile, line)) {
        if (line.empty() || line[0] == '/') continue; // skip empty or comment lines
        stringstream ss(line);
        string field;
        vector<string> fields;
        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }
        if (fields.size() < 11) continue;

        string type = fields[1];
        string trade_dt = fields[2];
        string start_dt = fields[3];
        string end_dt = fields[4];
        double notional = stod(fields[5]);
        string instrument = fields[6];
        double rate = stod(fields[7]);
        double strike = stod(fields[8]);
        double freq = stod(fields[9]);
        string optionType = fields[10];

        // Parse dates
        Date tradeDate, startDate, endDate;
        sscanf(trade_dt.c_str(), "%d-%d-%d", &tradeDate.year, &tradeDate.month, &tradeDate.day);
        sscanf(start_dt.c_str(), "%d-%d-%d", &startDate.year, &startDate.month, &startDate.day);
        sscanf(end_dt.c_str(), "%d-%d-%d", &endDate.year, &endDate.month, &endDate.day);

        if (type == "swap") {
            myPortfolio.push_back(new Swap(tradeDate, startDate, endDate, notional, rate, freq));
        } else if (type == "bond") {
            myPortfolio.push_back(new Bond(instrument, tradeDate, startDate, endDate, notional, freq, rate, 100.0));
        } else if (type == "european") {
            OptionType optType = (optionType == "call") ? OptionType::Call : OptionType::Put;
            myPortfolio.push_back(new EuropeanOption(instrument, optType, strike, endDate));
        } else if (type == "american") {
            OptionType optType = (optionType == "call") ? OptionType::Call : OptionType::Put;
            myPortfolio.push_back(new AmericanOption(instrument, optType, strike, endDate));
        }
    }
    tradeFile.close();

    // Price and output results
    std::ofstream out("zz_portfolio_valuation.txt");
    out << std::fixed << std::setprecision(4);

    for (auto trade : myPortfolio) {
        Pricer* pricer = nullptr;

        if (dynamic_cast<Bond*>(trade)) {
            pricer = new BondPricer();
        } else if (dynamic_cast<Swap*>(trade)) {
            pricer = new SwapPricer();
        } else if (dynamic_cast<EuropeanOption*>(trade) || dynamic_cast<AmericanOption*>(trade)) {
            pricer = new CRRBinomialTreePricer(treeTimeSteps);
        }

        if (pricer) {
            double pv = pricer->Price(mkt, trade);

            std::string typeName = typeid(*trade).name();
            typeName.erase(std::remove_if(typeName.begin(), typeName.end(), ::isdigit), typeName.end());
            out << "Trade Type: " << typeName << "\n";
            out << "PV: " << pv << "\n";

            if (auto bond = dynamic_cast<Bond*>(trade)) {
                out << "Bond Name: " << bond->getName() << "\n";
                out << "Maturity: " << bond->getMaturity() << "\n";
                out << "Coupon: " << bond->getCouponRate() << "\n";
            } else if (auto swap = dynamic_cast<Swap*>(trade)) {
                out << "Swap Notional: " << swap->getNotional() << "\n";
                out << "Start: " << swap->getStartDate() << ", End: " << swap->getEndDate() << "\n";
                out << "Fixed Rate: " << swap->getFixedRate() << "\n";
            } else if (auto euro = dynamic_cast<EuropeanOption*>(trade)) {
                out << "Option Ticker: " << euro->getTickerName() << "\n";
                out << "Strike: " << euro->getStrike() << "\n";
                out << "Expiry: " << euro->getExpiry() << "\n";

                BlackScholesPricer bsPricer;
                double bsPV = bsPricer.Price(mkt, trade);
                out << "Black-Scholes PV: " << bsPV << "\n";
            } else if (auto amer = dynamic_cast<AmericanOption*>(trade)) {
                out << "Option Ticker: " << amer->getTickerName() << "\n";
                out << "Strike: " << amer->getStrike() << "\n";
                out << "Expiry: " << amer->getExpiry() << "\n";
            }
            out << "-----------------------------\n";

            delete pricer;
        }
    }
    out.close();

	//task 4, analyzing pricing result
	// a) compare CRR binomial tree result for an european option vs Black model
	// b) compare CRR binomial tree result for an american option call vs european option call, and put

	for (auto trade : myPortfolio) {
		delete trade;
	}
	myPortfolio.clear();

	//final
	cout << "Project build successfully!" << endl;
	return 0;

}
