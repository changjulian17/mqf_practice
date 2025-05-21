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
	string curveFile = "curve.txt";
	string volFile = "vol.txt";	
	string bondFile = "bondPrice.txt";
	string stockFile = "stockPrice.txt";

	Market mkt = buildMarket(valueDate, curveFile, volFile, bondFile, stockFile);

    mkt.Print(); // print out the market data


	//task 2, create a portfolio of bond, swap, european option, american option
	//for each time, at least should have long / short, different tenor or expiry, different underlying
	//totally no less than 16 trades
	vector<Trade*> myPortfolio;										
	Trade* bond = new Bond("SGD-MAS-BILL", valueDate, valueDate, valueDate + "2Y", 100'000, 2, 2.5, 101.5);
	myPortfolio.push_back(bond);
	Trade* swap = new Swap(valueDate, valueDate + "1Y", valueDate + "5Y", 1'000'000, 0.045, 2);
	myPortfolio.push_back(swap);
	Trade* euroCall = new EuropeanOption("AAPL", OptionType::Call, 1.05, valueDate + "6M");
	myPortfolio.push_back(euroCall);


	//task 3, create a pricer and price the portfolio, output the pricing result of each deal.
	for (auto trade : myPortfolio) {
		Pricer* pricer = nullptr;

		if (dynamic_cast<Bond*>(trade)) {
			pricer = new BondPricer();
		} else if (dynamic_cast<Swap*>(trade)) {
			pricer = new SwapPricer();
		} else if (dynamic_cast<EuropeanOption*>(trade) || dynamic_cast<AmericanOption*>(trade)) {
			pricer = new CRRBinomialTreePricer(10);
		}

		if (pricer) {
			double pv = pricer->Price(mkt, trade);
			// log pv details out in a file
			delete pricer;
		}
	}

	//task 4, analyzing pricing result
	// a) compare CRR binomial tree result for an european option vs Black model
	// b) compare CRR binomial tree result for an american option call vs european option call, and put

	//final
	cout << "Project build successfully!" << endl;
	return 0;

}
