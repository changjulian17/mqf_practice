#include "PortfolioUtils.h"
#include "EuropeanTrade.h"
#include "helper.h" // for outputToFile and split
#include "Factory.h"
#include <algorithm>

TradeResult computePortfolioTotals(const std::vector<TradeResult>& results) {
    TradeResult total;
    total.id = 1000;
    total.tradeInfo = "Total";
    total.PV = 0.0;
    total.BlackPV = 0.0;
    total.Vega = 0.0;
    std::map<std::string, double> totalDV01;

    for (const auto& re : results) {
        total.PV += re.PV;
        total.BlackPV += re.BlackPV;
        total.Vega += re.Vega;
        for (const auto& kv : re.DV01) {
            totalDV01[kv.first] += kv.second;
        }
    }
    total.DV01 = totalDV01;
    return total;
}

void pricePortfolio(
    std::vector<TradeResult>& results,
    const std::vector<std::shared_ptr<Trade>>& myPortfolio,
    std::shared_ptr<Pricer> pricer,
    std::shared_ptr<Pricer> bsPricer,
    std::shared_ptr<Market> mkt
) {
    results.clear();
    for (size_t i = 0; i < myPortfolio.size(); i++) {
        auto& trade = myPortfolio[i];
        TradeResult re;
        re.id = i + 1;
        re.tradeInfo = trade->getType() + " " + trade->getUnderlying();
        re.PV = pricer->Price(*mkt, trade);
        if (auto euro = dynamic_cast<EuropeanOption*>(trade.get())) {
            re.BlackPV = bsPricer->Price(*mkt, trade) * trade->getNotional();
        }
        results.push_back(re);
    }
}

void computeGreeksForPortfolio(
    std::vector<TradeResult>& results,
    const std::vector<std::shared_ptr<Trade>>& myPortfolio,
    RiskEngine& re
) {
    for (size_t i = 0; i < myPortfolio.size(); ++i) {
        auto& trade = myPortfolio[i];

        re.computeRisk("dv01", trade, true);
        auto risk_re1 = re.getResult();
        std::map<std::string, double> dv01_map;
        for (const auto& kv : risk_re1) {
            if (kv.first == "USD-SOFR" || kv.first == "SGD-SORA") {
                dv01_map[kv.first] = kv.second;
            }
        }

        re.computeRisk("vega", trade, true);
        auto risk_re2 = re.getResult();
        std::map<std::string, double> vega_map;
        for (const auto& kv : risk_re2) {
            if (kv.first == "LOGVOL") {
                vega_map[kv.first] = kv.second;
            }
        }

        results[i].DV01 = dv01_map;
        results[i].Vega = vega_map.empty() ? 0.0 : vega_map.begin()->second;
    }
}

void outPutResult(const std::vector<TradeResult>& results)
{
    std::vector<std::string> output;
    size_t i = 0;
    for (const auto& re : results) {
        i++;
        std::string dv01_str = "{";
        for (auto it = re.DV01.begin(); it != re.DV01.end(); ++it) {
            if (it != re.DV01.begin()) dv01_str += ", ";
            dv01_str += it->first + ":" + std::to_string(it->second);
        }
        dv01_str += "}";

        std::string row;
        row = std::to_string(re.id) + "; " + re.tradeInfo +
              "; PV:" + std::to_string(re.PV) +
              "; BlackPV:" + std::to_string(re.BlackPV) +
              "; Delta:" + dv01_str +
              "; Vega:" + std::to_string(re.Vega);
        output.push_back(row);
    }
    outputToFile("output.txt", output);
}

void loadTrade(std::vector<std::shared_ptr<Trade>>& myPortfolio)
{
    std::string fileName = "trade.txt";
    std::string header;
    std::vector<std::string> tradeData;
    readFromFile(fileName, header, tradeData);
    std::vector<std::string> tradeHeader = split(header, ";");
    for (size_t i = 0; i < tradeData.size(); i++) {
        std::vector<std::string> tradeInfo = split(tradeData[i], ";");
        int id = stoi(tradeInfo[0]);
        std::string type = tradeInfo[1];
        Date tradeDate = Date(tradeInfo[2]);
        Date startDate = Date(tradeInfo[3]);
        Date endDate = Date(tradeInfo[4]);
        double notional = stod(tradeInfo[5]);
        std::string underlying = tradeInfo[6];
        double rate = stod(tradeInfo[7]);
        double strike = stod(tradeInfo[8]);
        double freq = stod(tradeInfo[9]);
        std::string optionTypeStr = tradeInfo[10];
        std::string directionStr = tradeInfo[11];
        directionStr.erase(std::remove(directionStr.begin(), directionStr.end(), '\r'), directionStr.end());

        OptionType optionType = OptionType::None;
        if (optionTypeStr == "call")
            optionType = OptionType::Call;
        else if (optionTypeStr == "put")
            optionType = OptionType::Put;
        else
            optionType = OptionType::None;

        DirectionType direction = DirectionType::NoneDir;
        if (directionStr == "pay")
            direction = DirectionType::Pay;
        else if (directionStr == "receive")
            direction = DirectionType::Receive;
        else if (directionStr == "long")
            direction = DirectionType::Long;
        else if (directionStr == "short")
            direction = DirectionType::Short;
        else
            direction = DirectionType::NoneDir;

        std::shared_ptr<Trade> trade;
        if (type == "bond") {
            auto bFactory = std::make_unique<BondFactory>();
            trade = bFactory->createTrade(underlying, startDate, endDate, notional, rate, strike, freq, optionType, direction);
        }
        else if (type == "swap") {
            auto sFactory = std::make_unique<SwapFactory>();
            trade = sFactory->createTrade(underlying, startDate, endDate, notional, rate, strike, freq, optionType, direction);
        }
        else if (type == "european") {
            auto eFactory = std::make_unique<EurOptFactory>();
            trade = eFactory->createTrade(underlying, startDate, endDate, notional, rate, strike, freq, optionType, direction);
        }
        else if (type == "american") {
            auto aFactory = std::make_unique<AmericanOptFactory>();
            trade = aFactory->createTrade(underlying, startDate, endDate, notional, rate, strike, freq, optionType, direction);
        }
        myPortfolio.push_back(trade);
    }
}