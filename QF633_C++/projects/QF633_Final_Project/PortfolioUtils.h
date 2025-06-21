#pragma once
#include <vector>
#include <memory>
#include "Trade.h"
#include "Pricer.h"
#include "RiskEngine.h"

void loadTrade(std::vector<std::shared_ptr<Trade>>& myPortfolio);

void pricePortfolio(
    std::vector<TradeResult>& results,
    const std::vector<std::shared_ptr<Trade>>& myPortfolio,
    std::shared_ptr<Pricer> pricer,
    std::shared_ptr<Pricer> bsPricer,
    std::shared_ptr<Market> mkt
);

TradeResult computePortfolioTotals(const std::vector<TradeResult>& results);

void computeGreeksForPortfolio(
    std::vector<TradeResult>& results,
    const std::vector<std::shared_ptr<Trade>>& myPortfolio,
    RiskEngine& re
);

void outPutResult(const std::vector<TradeResult>& results);