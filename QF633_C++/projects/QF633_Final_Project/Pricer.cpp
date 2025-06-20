#include <cmath>
#include "Pricer.h"
#include "Types.h"
#include "EuropeanTrade.h"

inline double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

double Pricer::Price(const Market& mkt, std::shared_ptr<Trade> trade)
{
	double pv;
	if (trade->getType() == "TreeProduct") {
		auto treePtr = dynamic_cast<TreeProduct*>(trade.get());
		if (treePtr) {
            int dir = 0;
            switch (treePtr->getDirection()) {
                case DirectionType::Long:
                    dir = 1;
                    break;
                case DirectionType::Short:
                    dir = -1;
                    break;
                default:
                    dir = 0;
                    break;
            }
            pv = dir * PriceTree(mkt, *treePtr) * trade->getNotional();
		}
	}
	else {
		pv = trade->Pv(mkt);
	}
	return pv;
}

double BinomialTreePricer::PriceTree(const Market& mkt, const TreeProduct& trade)
{
	// model setup
	double T = (trade.GetExpiry() - mkt.asOf)/365.0;
	double dt = T / nTimeSteps;
	double s0 = mkt.getStockPrice(trade.getUnderlying());
	auto volCurve = mkt.getVolCurve("LOGVOL");
	double vol = volCurve->getVol(trade.GetExpiry());
	auto irCurve = mkt.getCurve("USD-SOFR");
	double rate = irCurve->getRate(trade.GetExpiry());
	ModelSetup(s0, vol, rate, dt);

	// terminal payoff
	for (int i = 0; i <= nTimeSteps; i++) {
		states[i] = trade.Payoff(GetSpot(nTimeSteps, i));
	}

	// price by backward induction
	for (int k = nTimeSteps - 1; k >= 0; k--)
		for (int i = 0; i <= k; i++) {
			// calculate continuation value
			double df = exp(-rate * dt);
			double continuation = df * (states[i] * GetProbUp() + states[i + 1] * GetProbDown());
			// calculate the option value at node(k, i)
			states[i] = trade.ValueAtNode(GetSpot(k, i), dt * k, continuation);
		}

	return states[0];

}

void CRRBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
	double b = std::exp((2 * rate + sigma * sigma) * dt) + 1;
	u = (b + std::sqrt(b * b - 4 * std::exp(2 * rate * dt))) / 2 / std::exp(rate * dt);
	p = (std::exp(rate * dt) - 1 / u) / (u - 1 / u);
	currentSpot = S0;
}

void JRRNBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
	u = std::exp((rate - sigma * sigma / 2) * dt + sigma * std::sqrt(dt));
	d = std::exp((rate - sigma * sigma / 2) * dt - sigma * std::sqrt(dt));
	p = (std::exp(rate * dt) - d) / (u - d);
	currentSpot = S0;
}

double BlackScholesPricer::Price(const Market& mkt, std::shared_ptr<Trade> trade)
{
	auto euro = std::dynamic_pointer_cast<EuropeanOption>(trade);
	if (!euro) return 0.0;
	double T = (euro->GetExpiry() - mkt.asOf)/365.0;
	double s0 = mkt.getStockPrice(euro->getUnderlying());
	auto volCurve = mkt.getVolCurve("LOGVOL");
	double vol = volCurve->getVol(euro->GetExpiry());
	auto irCurve = mkt.getCurve("USD-SOFR");
	double rate = irCurve->getRate(euro->GetExpiry());
	double strike = euro->getStrike();
	bool type =  euro->getOptionType();

	int dir = 0;
	switch (euro->getDirection()) {
		case DirectionType::Long:
			dir = 1;
			break;
		case DirectionType::Short:
			dir = -1;
			break;
		default:
			dir = 0;
			break;
	}

	double d1 = (log(s0 / strike) + (rate + 0.5 * vol * vol) * T) / (vol * sqrt(T));
	double d2 = d1 - vol * sqrt(T);
	double df = exp(-rate * T);

	if (type == OptionType::Call)
		return dir * (s0 * norm_cdf(d1) - strike * df * norm_cdf(d2));
	else if (type == OptionType::Put)
		return dir * (strike * df * norm_cdf(-d2) - s0 * norm_cdf(-d1));
	else
		return 0.0;
}