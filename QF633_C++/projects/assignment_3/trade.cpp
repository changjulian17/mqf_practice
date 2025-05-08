#include "trade.h"
#include "black.h"
#include "date.h" // Include the Date class
#include <sstream>

// Define the virtual destructor for the trade class
trade::~trade() {}

// Constructor for OptionTrade that calculates expiry
OptionTrade::OptionTrade(double notional, double strike, const Date& start, const Date& end, bool isCall)
    : m_notional(notional), m_strike(strike), m_isCall(isCall), m_start(start), m_end(end) {
    m_expiry = m_end - m_start; // Calculate expiry using Date class
    cout << "OptionTrade constructor is called. Expiry: " << m_expiry << " years" << endl;
}

// Calculate PV using Black-Scholes model
double OptionTrade::calculatePv(double spot, double vol, double rate) const {
    double pv = BlackScholes(m_notional, m_strike, m_expiry, spot, vol, rate, m_isCall);
    return pv;
}

// Get trade details as a string
string OptionTrade::getTradeDetails() const {
    stringstream ss;
    ss << "Notional: " << m_notional
       << ", Strike: " << m_strike
       << ", Is Call: " << (m_isCall ? "true" : "false")
       << ", Start Date: " << m_start.getYear() << "-" << m_start.getMonth() << "-" << m_start.getDay()
       << ", End Date: " << m_end.getYear() << "-" << m_end.getMonth() << "-" << m_end.getDay()
       << ", Expiry: " << m_expiry << " years";
    return ss.str();
}