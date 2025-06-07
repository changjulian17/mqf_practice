#pragma once
#include "Trade.h"
#include "Market.h"

class Bond : public Trade {

public:
    Bond(const std::string& name, const Date& tradeDate, const Date& start, const Date& end, 
         double notional, int couponFreq, double coupon, double price)
        : Trade("BondTrade", tradeDate),
          bondName(name),
          bondNotional(notional),
          tradePrice(price),
          couponRate(coupon),
          frequency(couponFreq),
          startDate(start),
          endDate(end)
    {
        // No additional logic needed; just initialize members
    }

    std::string getName() const { return bondName; }
    double getNotional() const { return bondNotional; }
    Date getMaturity() const { return endDate; }
    double getCouponRate() const { return couponRate; }
    int getFrequency() const { return frequency; }

    inline void setStartDate(const Date& d) { startDate = d; }
    inline void setEndDate(const Date& d) { endDate = d; }
    inline void setNotional(double n) { bondNotional = n; }
    inline void setCouponRate(double r) { couponRate = r; }
    inline void setFrequency(int f) { frequency = f; }
    inline void setTradePrice(double p) { tradePrice = p; }
    inline void setName(const std::string& n) { bondName = n; }


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

