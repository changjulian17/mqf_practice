#pragma once
#include "Trade.h"

class Bond : public Trade {
public:
    Bond(std::string name, Date start, Date end, double _notional, double rate, double freq)
    {
        tradeType = "Bond";
        underlying = name;
        notional = _notional;
		startDate = start;
        tradeDate = start;
        frequency = freq;
        coupon = rate;
    }
    inline string getType() const { return tradeType; };
    inline string getUnderlying() const { return underlying; };
	void setTradePrice(double price) { tradePrice = price; }
    double Payoff(double s) const; // implement this
    double Pv(const Market& mkt) const; // implement this
    void generateSwapSchedule(); //implement this

private:
    
    double tradePrice;
    double coupon;
    double frequency;
    Date startDate;
    Date maturityDate;
    vector<Date> bondSchedule;

};