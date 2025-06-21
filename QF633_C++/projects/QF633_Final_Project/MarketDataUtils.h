#pragma once

#include "Market.h"

void loadIrCurve(Market& mkt, const std::string& fileName, const std::string& curveName);
void loadVolCurve(Market& mkt, const std::string& fileName, const std::string& curveName);
void loadStockPrices(Market& mkt, const std::string& fileName);