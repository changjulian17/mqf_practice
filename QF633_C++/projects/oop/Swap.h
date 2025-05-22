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

	double getNotional() const { return swapNotional; }
	Date getStartDate() const { return startDate; }
	Date getEndDate() const { return endDate; }
	double getFixedRate() const { return tradeRate; }

	// Now Payoff takes a RateCurve
	double Payoff(double marketRate) const
	{
		return swapNotional * (tradeRate - marketRate);
	}

	// Now Payoff takes a RateCurve
	double Payoff(const RateCurve& curve) const
	{	
		double annuity = 0.0;
		double sumRates = 0.0;
		double sumFwdRates = 0.0;
		double years = endDate.toDouble() - startDate.toDouble();
		int nPeriods = static_cast<int>(round(frequency * years));
		double dt = 1.0 / frequency;

		for (int i = 1; i <= nPeriods; ++i) {
			double t = i * dt;
			// Compute payDate as startDate + t years (or months)
			Date payDate = startDate;
			int monthsToAdd = static_cast<int>(round(12 * t));
			payDate.month += monthsToAdd;
			while (payDate.month > 12) {
				payDate.year += 1;
				payDate.month -= 12;
			}
			double z1 = curve.getRate(payDate);
			double df = exp(-z1 * t);
			annuity += swapNotional * dt * df;
		}

		double marketSwapRate = curve.getMarketSwapRate(startDate, endDate, frequency);  
		return annuity * (tradeRate - marketSwapRate);
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