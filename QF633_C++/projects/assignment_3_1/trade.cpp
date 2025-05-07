#include "trade.h"

OptionTrade::OptionTrade(double notional, double strike, double expiry, bool isCall)
{


};

double OptionTrade::calculatePv(double spot, double vol, double rate) const
{
    double pv = 0;
    /*
    call Black Sholes model here
    */

    return pv;
};

// string OptionTrade::getTradeDetails() const
// {

// }