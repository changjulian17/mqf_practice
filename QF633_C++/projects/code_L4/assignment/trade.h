#pragma once

#include <string>
#include <iostream>
#include "date.h"

using namespace std;

//complete this implementation
class OptionTrade
{
public:

    OptionTrade() {
        cout << "deafult constructor is called for option trade" << endl;
    };

    ~OptionTrade() {
        cout << "deafult destructor is called for option trade" << endl;
    }

    OptionTrade(double notional, double strike, double expiry, bool isCall);


    double calculatePv(double spot, double vol, double rate) const;

    string getTradeDetails() const;


private:
    double m_notional = 0;
    double m_strike = 0;    
    bool m_isCall = true;
    Date m_start;
    Date m_end;
};



