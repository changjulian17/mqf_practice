#include "trade.h"
#include "black.h"
#include <sstream>

// Define the virtual destructor for the trade class
trade::~trade() {}

// Constructor for OptionTrade
OptionTrade::OptionTrade(double notional, double strike, double expiry, bool isCall)
    : m_notional(notional), m_strike(strike), m_isCall(isCall) {
    cout << "OptionTrade constructor is called." << endl;
}

// Calculate PV using Black-Scholes model
double OptionTrade::calculatePv(double spot, double vol, double rate) const {
    double pv = BlackScholes(m_notional, m_strike, 1, spot, vol, rate, m_isCall);
    return pv;
}

// Get trade details as a string
string OptionTrade::getTradeDetails() const {
    stringstream ss;
    ss << "Notional: " << m_notional
       << ", Strike: " << m_strike
       << ", Is Call: " << (m_isCall ? "true" : "false");
    return ss.str();
}