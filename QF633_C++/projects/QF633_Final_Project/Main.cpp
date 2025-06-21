#include <fstream>
#include <ctime>
#include <chrono>
#include <map>

#include "Market.h"
#include "Pricer.h"
#include "RiskEngine.h"
#include "Factory.h"
#include "thread_pool.h"
#include "helper.h"
#include "PortfolioUtils.h"
#include "MarketDataUtils.h"

using namespace std;


int main()
{
	// Get the current system time
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm localTime;
	localtime_r(&t, &localTime);
	Date valueDate = Date(localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday);

	// step1: create market data and load curve, vol and prices into market data
	auto mkt = make_shared<Market>(valueDate);
	loadIrCurve(*mkt, "usd_curve.txt", "USD-SOFR");
	loadIrCurve(*mkt, "sgd_curve.txt", "SGD-SORA");
	loadVolCurve(*mkt, "vol.txt", "LOGVOL");
	loadStockPrices(*mkt, "stockPrice.txt");
	// mkt->addStockPrice("APPL", 652.0);
	// mkt->addStockPrice("SP500", 5035.7);
	// mkt->addStockPrice("STI", 3420);

	mkt->Print();
	auto usdCurve = mkt->getCurve("USD-SOFR");
	double rate1 = usdCurve->getRate(Date(2026, 1, 1));
	cout << "usd curve rate:" << rate1 << endl;
	auto sgdCurve = mkt->getCurve("SGD-SORA");
	double rate2 = sgdCurve->getRate(Date(2026, 4, 15));
	cout << "sgd curve rate:" << rate2 << endl;

	//step 2, create a portfolio of bond, swap, european option, american option
	vector<std::shared_ptr<Trade>> myPortfolio;
	loadTrade(myPortfolio);
	auto sFactory = std::make_unique<SwapFactory>();
	auto eFactory = std::make_unique<EurOptFactory>();
	auto swap = sFactory->createTrade("USD-SOFR", Date(2024, 1, 1), Date(2034, 1, 1), -1000000, .05, 0.03, 1.0, OptionType::None, DirectionType::Pay);
	auto eCall = eFactory->createTrade("APPL", Date(2024, 1, 1), Date(2025, 1, 1), 10000, 0, 530, 0, OptionType::Call, DirectionType::Long);

	// step 3, create a pricer and price the portfolio, output the pricing result of each deal 
	vector<TradeResult> results;
	auto pricer = make_shared<CRRBinomialTreePricer>(50);
	auto bsPricer = std::make_shared<BlackScholesPricer>();
	pricePortfolio(results, myPortfolio, pricer, bsPricer, mkt);

	//task 4, compute the Greeks of DV01, and Vega risk as of market date 1
	// 4.1 compute risk using risk engine
	// 4.2 use idea of multi-threading
	// analyzing the pv and risk

	// sample code for risk computation
	double curve_shock = 0.0001;// 1 bp of zero rate
	double vol_shock = 0.01; //1% of log normal vol
	double price_shock = 1.0; // shock in abs price of stock

	// example 1, simple example of computing one point dv01 for one swap
	// string risk_id = "USD-SOFR:DV01:DEAL 01";
	// double shockUp = 0.0001;
	// double shockDown = -0.0001;
	// auto testShockUp = MarketShock();
	// testShockUp.market_id = "USD-SOFR";
	// testShockUp.shock = make_pair(Date(), shockUp);
	// auto testShockDown = MarketShock();
	// testShockDown.market_id = "USD-SOFR";
	// testShockDown.shock = make_pair(Date(), shockDown);
	// auto shockedUpCurveUp = CurveDecorator(*mkt, testShockUp);

	// unordered_map<string, double> thisDealDv01;
	// double pv_up, pv_down;
	// auto m_up = shockedUpCurveUp.getMarketUp();
	// auto m_down = shockedUpCurveUp.getMarketDown();
	// pv_up = swap->Pv(m_up);
	// pv_down = swap->Pv(m_down);
	// double dv01 = (pv_up - pv_down) / 2.0;
	// thisDealDv01.emplace(risk_id, dv01);

	//example2, using risk engine to compute full set of dv01 for a swap
	RiskEngine re(*mkt, curve_shock, vol_shock, price_shock);
	// Compute DV01 for all instruments in the portfolio and update Delta in results
	computeGreeksForPortfolio(results, myPortfolio, re);

	//example 3, demo using thread pool
	if (false){
		map<string, double> swapDv01;
		ThreadPool pool(4);
	
		// auto pv_job = [&swapDv01, risk_id, &swap, &m_up, &m_down]() {
		// cout << "Task is running on thread: " << this_thread::get_id() << endl;
		// auto pricer = std::make_unique<CRRBinomialTreePricer>(100);
		// double pv_u = pricer->Price(m_up, swap);
		// double pv_d = pricer->Price(m_down, swap);
		// double dv01 = (pv_u - pv_d) / 2.;
		// swapDv01.emplace(std::make_pair(risk_id, dv01));
		// this_thread::sleep_for(chrono::milliseconds(100));
		// };

		// for (int i = 0; i < 5; ++i) {
		// 	pool.enqueue(pv_job);
		// }

		// auto pv_job = [&swapDv01, risk_id, &eCall, &m_up, &m_down]() {
		// cout << "Task is running on thread: " << this_thread::get_id() << endl;
		// auto pricer = std::make_unique<CRRBinomialTreePricer>(100);
		// double pv_u = pricer->Price(m_up, eCall);
		// double pv_d = pricer->Price(m_down, eCall);
		// double dv01 = (pv_u - pv_d) / 2.;
		// swapDv01.emplace(std::make_pair(risk_id, dv01));
		// this_thread::sleep_for(chrono::milliseconds(100));
		// };

		// for (int i = 0; i < 5; ++i) {
		// 	pool.enqueue(pv_job);
		// }
	}

	// step 5, output result to file
	// --- Compute totals ---
	TradeResult total = computePortfolioTotals(results);
	results.push_back(total);
	outPutResult(results);

	//final
	cout << "Project build successfully!" << endl;

	return 0;

}
