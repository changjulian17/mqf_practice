#pragma once

#include "Types.h"

//option type of trade, will be priced using tree model
class TreeProduct: public Trade
{
public:
    TreeProduct(): Trade() { tradeType = "TreeProduct";};
    virtual const Date& GetExpiry() const = 0;
    virtual double ValueAtNode(double stockPrice, double t, double continuationValue) const = 0;
    virtual DirectionType getDirection() const { return DirectionType::NoneDir; }
    double Pv(const Market& mkt) const { return 0; }; 
};
