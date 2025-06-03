#include <cmath>
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "AmericanTrade.h"


double Pricer::Price(const Market& mkt, Trade* trade) {
	double pv = 0;
	if (trade->getType() == "TreeProduct") {
		TreeProduct* treePtr = dynamic_cast<TreeProduct*>(trade);		// dynamic_cast is used to safely cast a pointer to a derived class
		if (treePtr) { //check if cast is sucessful
			pv = PriceTree(mkt, *treePtr);
		}
	}
	else {
		double price = 0; //get from market data
		pv = trade->Payoff(price);
	}

	return pv;
}

void BinomialTreePricer::ModelSetup(double S0, double sigma, double r, double dt) 
{
	// a basic version of binomial tree
	u = 2;
	d = 0.5;
	p = (exp(r * dt) - d) / (u - d);
}

double BinomialTreePricer::PriceTree(const Market& mkt, const TreeProduct& trade) {
	double T = trade.GetExpiry() - mkt.asOf;
	double dt = T / nTimeSteps;
	double stockPrice = 0, vol = 0, rate = 0;
	std::string ticker;

	// Try to get ticker and other info from EuropeanOption
	if (auto euro = dynamic_cast<const EuropeanOption*>(&trade)) {
		ticker = euro->getTickerName();
	}
	// Try to get ticker and other info from AmericanOption
	else if (auto amer = dynamic_cast<const AmericanOption*>(&trade)) {
		ticker = amer->getTickerName();
	}

	// Now fill market data if ticker is available
	if (!ticker.empty()) {
		stockPrice = mkt.getStockPrice();
		// if have multiple stocks, use the ticker in 
		// getVolCurve to get the specific stock price
		vol = mkt.getVolCurve().getVol(trade.GetExpiry()); 
		rate = .04;
		// rate = mkt.getCurve().getRate(trade.GetExpiry());
	}

	ModelSetup(stockPrice, vol, rate, dt);

	// initialize
	for (int i = 0; i <= nTimeSteps; i++) {
		states[i] = trade.Payoff(GetSpot(nTimeSteps, i));
	}

	// price by backward induction
	for (int k = nTimeSteps - 1; k >= 0; k--){
		for (int i = 0; i <= k; i++) {
			double df = exp(-rate * dt);
			double continuation = df * (states[i] * GetProbUp() + states[i + 1] * GetProbDown());
			states[i] = trade.ValueAtNode(GetSpot(k, i), dt * k, continuation);
		}	
	}
	return states[0];

}

void CRRBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
	rate = .04;
	u = exp(sigma * sqrt(dt));
	d = exp(-sigma * sqrt(dt));
	p = (exp(rate * dt) - d) / (u - d);
	currentSpot = S0;
}

void JRRNBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
	rate = .04;
	u = std::exp((rate - sigma * sigma / 2) * dt + sigma * std::sqrt(dt));
	d = std::exp((rate - sigma * sigma / 2) * dt - sigma * std::sqrt(dt));
	p = (std::exp(rate * dt) - d) / (u - d);
	currentSpot = S0;
}


double norm_cdf(double x) {
    return 0.5 * erfc(-x * M_SQRT1_2);
}

double BlackScholesPricer::Price(const Market& mkt, Trade* trade) {
	// Try to cast to either option type and handle separately
	if (auto euro = dynamic_cast<EuropeanOption*>(trade)) {
		std::string ticker = euro->getTickerName();
		double spot = mkt.getStockPrice();
		double vol = mkt.getVolCurve().getVol(euro->getExpiry());
		double rate = .04;
		// double rate = mkt.getCurve().getRate(euro->getExpiry());
		double strike = euro->getStrike();
		double expiry = euro->getExpiry() - mkt.asOf;
		bool type =  euro->getOptionType();

		double d1 = (log(spot / strike) + (rate + 0.5 * vol * vol) * expiry) / (vol * sqrt(expiry));
		double d2 = d1 - vol * sqrt(expiry);
		double df = exp(-rate * expiry);

		if (type == OptionType::Call)
			return spot * norm_cdf(d1) - strike * df * norm_cdf(d2);
		else if (type == OptionType::Put)
			return strike * df * norm_cdf(-d2) - spot * norm_cdf(-d1);
		else
			return 0.0;
	}
	return 0.0;
}