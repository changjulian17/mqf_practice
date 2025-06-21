#pragma once
#include <string>
#include "Market.h"

// Load an interest rate curve from file and add to market
void loadIrCurve(Market& mkt, const std::string& fileName, const std::string& curveName);

// Load a volatility curve from file and add to market
void loadVolCurve(Market& mkt, const std::string& fileName, const std::string& curveName);

// Load stock prices from file and add to market
void loadStockPrices(Market& mkt, const std::string& fileName);