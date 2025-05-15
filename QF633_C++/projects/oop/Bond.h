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

    inline double Payoff(double marketPrice) const override
    { 
        return (marketPrice - tradePrice) * bondNotional;
    }; // implement this as NPV including price * notional

    double Payoff(const RateCurve& curve) const
	{
		return 50.0; // TODO implement discounting cashflow
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

