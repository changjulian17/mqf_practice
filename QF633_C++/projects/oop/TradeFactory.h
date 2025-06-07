#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "Trade.h"
#include "Swap.h"
#include "Bond.h"
#include "EuropeanTrade.h"
#include "AmericanTrade.h"

using namespace std;

// Abstract creator class
class TradeFactory
{
public:
    virtual shared_ptr<Trade> createTrade(
        const string &type,
        const string &instrument,
        const Date &tradeDate,
        const Date &startDate,
        const Date &endDate,
        double notional,
        double rate,
        double freq,
        double strike,
        const string &optionType
    ) = 0;

    virtual ~TradeFactory()
    {
        cout << "trade factory is destroyed" << endl;
    }
};

// Concrete creator class - create linear trades
class LinearTradeFactory : public TradeFactory
{
public:
    shared_ptr<Trade> createTrade(
        const string &type,
        const string &instrument,
        const Date &tradeDate,
        const Date &startDate,
        const Date &endDate,
        double notional,
        double rate,
        double freq,
        double strike,
        const string &optionType
    ) override
    {
        if (type == "swap")
        {
            return make_shared<Swap>(tradeDate, startDate, endDate, notional, rate, freq);
        }
        else if (type == "bond")
        {
            return make_shared<Bond>(instrument, tradeDate, startDate, endDate, notional, freq, rate, 1.0);
        }
        else
            return nullptr;
    }
};

// Concrete creator class - create option trades
class OptionTradeFactory : public TradeFactory
{
public:
    shared_ptr<Trade> createTrade(
        const string &type,
        const string &instrument,
        const Date &tradeDate,
        const Date &startDate,
        const Date &endDate,
        double notional,
        double rate,
        double freq,
        double strike,
        const string &optionType
    ) override
    {
        OptionType optType = (optionType == "call") ? OptionType::Call : OptionType::Put;
        if (type == "european")
        {
            return make_shared<EuropeanOption>(instrument, optType, strike, endDate);
        }
        else if (type == "american")
        {
            return make_shared<AmericanOption>(instrument, optType, strike, endDate);
        }
        else
            return nullptr;
    }
};