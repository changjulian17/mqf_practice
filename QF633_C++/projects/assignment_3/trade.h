#pragma once

#include <string>
#include <iostream>
#include "date.h" // Include the Date class

using namespace std;

// Abstract base class trade
class trade {
public:
    virtual double calculatePv(double spot, double vol, double rate) const = 0;
    virtual string getTradeDetails() const = 0;

    virtual ~trade();
};

class OptionTrade : public trade {
public:
    OptionTrade() {
        cout << "Default constructor is called for OptionTrade" << endl;
    };

    ~OptionTrade() {
        cout << "Default destructor is called for OptionTrade" << endl;
    }

    // Constructor that calculates expiry
    OptionTrade(double notional, double strike, const Date& start, const Date& end, bool isCall);

    double calculatePv(double spot, double vol, double rate) const override;

    string getTradeDetails() const override;

private:
    double m_notional = 0;
    double m_strike = 0;
    bool m_isCall = true;
    double m_expiry = 0; // Expiry in years
    Date m_start;
    Date m_end;
};



