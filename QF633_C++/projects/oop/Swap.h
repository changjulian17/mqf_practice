#pragma once
#include "Trade.h"
#include "Market.h"
#include <cmath>

class Swap : public Trade
{
public:
	Swap(const Date &tradeDate, Date start, Date end, double notional, double rate, int freq)
		: Trade("SwapTrade", tradeDate), startDate(start), endDate(end), swapNotional(notional), tradeRate(rate), frequency(freq)
	{
		/*
		add constructor details
		*/
	}

	// Now Payoff takes a RateCurve
	double Payoff(double marketRate) const
	{
		return swapNotional * (tradeRate - marketRate);
	}

	// Now Payoff takes a RateCurve
	double Payoff(const RateCurve& curve) const
	{

		/*
		Implement this, using npv = annuity * (traded rate - market swap rate);
		trade rate is "rate"
		market swap rate is "marketRate" in curve.txt
		Annuity = sum of (notional * year fraction of each coupon period * Discount factor at each period end);
		Df = exp(-zT), z is the zero coupon rate;
		*/

		auto result = getAnnuityAndSumRates(curve);
		double annuity = result.first;
		double sumRates = result.second;
		int nPeriods = frequency * (endDate.year - startDate.year);

		// Multiply tradeRate by nPeriods as requested
		return annuity * (sumRates - tradeRate * nPeriods);
	}

private:
	Date startDate;
	Date endDate;
	double swapNotional;
	double tradeRate;
	int frequency; // use 1 for annual, 2 for semi-annual etc

	// Modify getAnnuity to also return the sum of rates z over the period
	std::pair<double, double> getAnnuityAndSumRates(const RateCurve& curve) const
	{
		double annuity = 0.0;
		double sumRates = 0.0;
		int nPeriods = frequency * (endDate.year - startDate.year);
		double dt = 1.0 / frequency;

		for (int i = 1; i <= nPeriods; ++i) {
			// Estimate payment date
			Date payDate = startDate;
			payDate.year += (i - 1) / frequency;
			payDate.month += (12 / frequency) * ((i - 1) % frequency);
			if (payDate.month > 12) {
				payDate.year += (payDate.month - 1) / 12;
				payDate.month = ((payDate.month - 1) % 12) + 1;
			}
			double t = payDate.toDouble() - startDate.toDouble();
			double z = curve.getRate(payDate); // get zero rate from curve
			double df = exp(-z * t); // continuous discounting
			annuity += swapNotional * dt * df;
			sumRates += z;
		}
		return std::make_pair(annuity, sumRates);
	}
};