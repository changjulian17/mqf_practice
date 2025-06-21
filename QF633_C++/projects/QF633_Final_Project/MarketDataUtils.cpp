#include "MarketDataUtils.h"
#include "helper.h"
#include <vector>
#include <fstream>
#include <iostream>

void loadIrCurve(Market& mkt, const std::string& fileName, const std::string& curveName)
{
    auto curve = std::make_shared<RateCurve>(curveName);
    std::string header;
    std::vector<std::string> curveData;
    readFromFile(fileName, header, curveData);
    Date valueDate = mkt.asOf; // use market date as value date for the curve
    curve->_asOf = valueDate;
    for (size_t i = 0; i < curveData.size(); i++) {
        std::vector<std::string> rateInfo = split(curveData[i], ":");
        std::string tenor = rateInfo[0];
        double rate = std::stod(rateInfo[1].substr(0, rateInfo[1].size() - 1)) / 100;
        Date tenorDate = dateAddTenor(valueDate, tenor);
        curve->addRate(tenorDate, rate);
    }
    mkt.addCurve(curveName, curve);
}

void loadVolCurve(Market& mkt, const std::string& fileName, const std::string& curveName)
{
    auto curve = std::make_shared<VolCurve>(curveName);
    std::string header;
    std::vector<std::string> curveData;
    readFromFile(fileName, header, curveData);
    Date valueDate = mkt.asOf; // use market date as value date for the curve
    curve->_asOf = valueDate;
    for (size_t i = 0; i < curveData.size(); i++) {
        std::vector<std::string> rateInfo = split(curveData[i], ":");
        std::string tenor = rateInfo[0];
        double vol = std::stod(rateInfo[1].substr(0, rateInfo[1].size() - 1)) / 100;
        Date tenorDate = dateAddTenor(valueDate, tenor);
        curve->addVol(tenorDate, vol);
    }
    mkt.addVolCurve(curveName, curve);
}

void loadStockPrices(Market& mkt, const std::string& fileName) {
    std::vector<std::string> stockData;
    std::string lineText;
    std::ifstream input_file(fileName);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open file '" << fileName << "'" << std::endl;
        return;
    }

    while (getline(input_file, lineText)) {
        stockData.push_back(lineText);
    }
    input_file.close();
    for (const auto& line : stockData) {
        std::vector<std::string> tokens = split(line, ":");
        if (tokens.size() >= 2) {
            std::string ticker = tokens[0];
            double price = std::stod(tokens[1]);
            mkt.addStockPrice(ticker, price);
        }
    }
}