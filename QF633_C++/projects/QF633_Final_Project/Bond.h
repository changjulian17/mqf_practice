#pragma once
#include "Trade.h"
#include "Types.h"

class Bond : public Trade {
public:
    Bond(std::string name, Date start, Date end, double _notional, double rate, double freq, DirectionType _direction)
    {
        tradeType = "Bond";
        underlying = to_upper(name);
        notional = _notional;
        tradeDate = start;
        startDate = start;
        maturityDate = end;
        frequency = freq;
        coupon = rate;
        rateCurve = to_upper(name).substr(0,3) == "SGD"? "SGD-SORA" : "USD-SOFR";
        direction = _direction;
        generateSchedule();
    }
    inline string getType() const { return tradeType; };
    inline string getUnderlying() const { return underlying; };
    inline double getNotional() const { return notional; }
    inline double getRate() const { return coupon; } 
    DirectionType getDirection() const { return direction; }
    void setTradePrice(double price) { tradePrice = price; }
    double Payoff(double s) const; // implement this
    double Pv(const Market& mkt) const; // implement this
    void generateSchedule(); //implement this

private:
    double tradePrice;
    double coupon;
    double frequency;
    Date startDate;
    Date maturityDate;
    vector<Date> bondSchedule;
    string rateCurve;
    DirectionType direction;
};