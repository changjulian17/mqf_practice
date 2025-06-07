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
#include "TradeFactory.h"

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
	
	int treeTimeSteps = 50;
	string curveFile = "curve.txt";
	string volFile = "vol.txt";	
	string bondFile = "bondPrice.txt";
	string stockFile = "stockPrice.txt";

    auto mkt = std::make_shared<Market>(buildMarket(valueDate, curveFile, volFile, bondFile, stockFile));

    mkt->Print(); // print out the market data

    vector<shared_ptr<Trade>> myPortfolio;
    auto linearFactory = std::make_unique<LinearTradeFactory>();
    auto optionFactory = std::make_unique<OptionTradeFactory>();

    ifstream tradeFile("trade.txt");
    string line;
    getline(tradeFile, line); // skip header

    while (getline(tradeFile, line)) {
        if (line.empty() || line[0] == '/') continue;
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

        Date tradeDate, startDate, endDate;
        sscanf(trade_dt.c_str(), "%d-%d-%d", &tradeDate.year, &tradeDate.month, &tradeDate.day);
        sscanf(start_dt.c_str(), "%d-%d-%d", &startDate.year, &startDate.month, &startDate.day);
        sscanf(end_dt.c_str(), "%d-%d-%d", &endDate.year, &endDate.month, &endDate.day);

        shared_ptr<Trade> tradePtr;
        if (type == "swap" || type == "bond") {
            tradePtr = linearFactory->createTrade(type, instrument, tradeDate, startDate, endDate, notional, rate, 1/freq, strike, optionType);
        } else if (type == "european" || type == "american") {
            tradePtr = optionFactory->createTrade(type, instrument, tradeDate, startDate, endDate, notional, rate, 1/freq, strike, optionType);
        }
        if (tradePtr) {
            myPortfolio.push_back(tradePtr);
        }
    }
    tradeFile.close();

    // Price and output results
    std::ofstream out("zz_portfolio_valuation.txt");
    out << std::fixed << std::setprecision(4);

    for (auto trade : myPortfolio) {
        std::shared_ptr<Pricer> pricer;

        if (dynamic_cast<Bond*>(trade.get())) {
            pricer = std::make_shared<BondPricer>();
        } else if (dynamic_cast<Swap*>(trade.get())) {
            pricer = std::make_shared<SwapPricer>();
        } else if (dynamic_cast<EuropeanOption*>(trade.get()) || dynamic_cast<AmericanOption*>(trade.get())) {
            pricer = std::make_shared<CRRBinomialTreePricer>(treeTimeSteps);
        }

        if (pricer) {
            double pv = pricer->Price(*mkt, trade.get()); // <-- dereference shared_ptr

            std::string typeName = typeid(*(trade.get())).name();
            typeName.erase(std::remove_if(typeName.begin(), typeName.end(), ::isdigit), typeName.end());
            out << "Trade Type: " << typeName << "\n";
            out << "PV: " << pv << "\n";

            if (auto bond = dynamic_cast<Bond*>(trade.get())) {
                out << "Bond Name: " << bond->getName() << "\n";
                out << "Maturity: " << bond->getMaturity() << "\n";
                out << "Coupon: " << bond->getCouponRate() << "\n";
            } else if (auto swap = dynamic_cast<Swap*>(trade.get())) {
                out << "Swap Notional: " << swap->getNotional() << "\n";
                out << "Start: " << swap->getStartDate() << ", End: " << swap->getEndDate() << "\n";
                out << "Fixed Rate: " << swap->getFixedRate() << "\n";
            } else if (auto euro = dynamic_cast<EuropeanOption*>(trade.get())) {
                out << "Option Ticker: " << euro->getTickerName() << "\n";
                out << "Strike: " << euro->getStrike() << "\n";
                out << "Expiry: " << euro->getExpiry() << "\n";

                BlackScholesPricer bsPricer;
                double bsPV = bsPricer.Price(*mkt, trade.get());
                out << "Black-Scholes PV: " << bsPV << "\n";
            } else if (auto amer = dynamic_cast<AmericanOption*>(trade.get())) {
                out << "Option Ticker: " << amer->getTickerName() << "\n";
                out << "Strike: " << amer->getStrike() << "\n";
                out << "Expiry: " << amer->getExpiry() << "\n";
            }
            out << "-----------------------------\n";
        }
    }
    out.close();

	//task 4, analyzing pricing result
	// a) compare CRR binomial tree result for an european option vs Black model
	// b) compare CRR binomial tree result for an american option call vs european option call, and put

    myPortfolio.clear();

	//final
	cout << "Project build successfully!" << endl;
	return 0;

}
