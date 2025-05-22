#pragma once
#include "Trade.h"
#include "Market.h"

class Bond : public Trade {

public:
    Bond(const std::string& name, const Date& tradeDate, const Date& start, const Date& end, 
        double notional, int couponFreq, double coupon, double price) // add couponRate here
        : Trade("BondTrade", tradeDate) {
        startDate = start;
		endDate = end;
		bondName = name;
        bondNotional = notional;
        frequency = couponFreq;
        couponRate = coupon; // set couponRate
        tradePrice = price;
    }

    std::string getName() const { return bondName; }
    double getNotional() const { return bondNotional; }
    Date getMaturity() const { return endDate; }
    double getCouponRate() const { return couponRate; }
    int getFrequency() const { return frequency; }

    inline double Payoff(double marketPrice) const override
    { 
        return (marketPrice - tradePrice) * bondNotional;
    }; // implement this as NPV including price * notional

    double Payoff(const RateCurve& curve) const
	{
        double npv = 0.0;
        // Estimate number of periods (assumes full years, adjust if needed)
        int nPeriods = frequency * (endDate.year - startDate.year);
        double periodCoupon = bondNotional * (couponRate / 100.0) / frequency;

        for (int i = 1; i <= nPeriods; ++i) {
            // Estimate payment date (very basic, assumes equal spacing)
            Date payDate = startDate;
            payDate.year += (i - 1) / frequency;
            payDate.month += (12 / frequency) * ((i - 1) % frequency);
            if (payDate.month > 12) {
                payDate.year += (payDate.month - 1) / 12;
                payDate.month = ((payDate.month - 1) % 12) + 1;
            }
            double t = payDate.toDouble() - startDate.toDouble();
            double rate = curve.getRate(payDate);
            double discount = exp(-rate * t); // continuous discounting
            npv += periodCoupon * discount;
        }
        // Add principal repayment at maturity
        double tMat = endDate.toDouble() - startDate.toDouble();
        double rateMat = curve.getRate(endDate);
        double discountMat = exp(-rateMat * tMat); // continuous discounting
        npv += bondNotional * discountMat;

        return npv;
	};

private:
    std::string bondName;
    double bondNotional;
    double tradePrice;
    double couponRate;      // add this member
    int frequency;          // typo fixed from 'frequecy'
    Date startDate;
    Date endDate;
};

