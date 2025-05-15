#include "Market.h"
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>

// Forward declaration of your helper
extern std::vector<std::pair<std::string, std::string>> readKeyValueFile(const std::string& fileName);

Market buildMarket(
    const Date& valueDate,
    const std::string& curveFile,
    const std::string& volFile,
    const std::string& bondFile,
    const std::string& stockFile
) {
    Market mkt(valueDate);

    auto curvePairs = readKeyValueFile(curveFile);
    auto volPairs = readKeyValueFile(volFile);
    auto bondPairs = readKeyValueFile(bondFile);
    auto stockPairs = readKeyValueFile(stockFile);

    RateCurve usdSofr("USD-SOFR");
    for (const auto& kv : curvePairs) {
        const std::string& tenor = kv.first;
        const std::string& rateStr = kv.second;
        double rate = std::stod(rateStr) / 100.0;
        Date tenorDate = valueDate + tenor;
        usdSofr.addRate(tenorDate, rate);
    }
    mkt.addCurve("USD-SOFR", usdSofr);

    VolCurve stockVol("STOCK-VOL");
    for (const auto& kv : volPairs) {
        const std::string& tenor = kv.first;
        const std::string& volStr = kv.second;
        double vol = std::stod(volStr) / 100.0;
        Date tenorDate = valueDate + tenor;
        stockVol.addVol(tenorDate, vol);
    }
    mkt.addVolCurve("STOCK-VOL", stockVol);

    for (const auto& kv : bondPairs) {
        const std::string& bondName = kv.first;
        const std::string& priceStr = kv.second;
        double price = std::stod(priceStr);
        mkt.addBondPrice(bondName, price);
    }

    for (const auto& kv : stockPairs) {
        const std::string& stockName = kv.first;
        const std::string& priceStr = kv.second;
        double price = std::stod(priceStr);
        mkt.addStockPrice(stockName, price);
    }

    return mkt;
}

#include "Market.h"

using namespace std;

void RateCurve::display() const
{
  cout << "rate curve:" << name << endl;
  for (size_t i = 0; i < tenorDates.size(); i++)
  {
    cout << tenorDates[i] << ":" << rates[i] << endl;
  }
  cout << endl;
}

void RateCurve::addRate(Date tenor, double rate)
{
  // consider to check if tenor already exist
  if (true)
  {
    tenorDates.push_back(tenor);
    rates.push_back(rate);
  }
}

double RateCurve::getRate(Date tenor) const
{
  // use linear interpolation to get rate
  return 0;
}

void VolCurve::addVol(Date tenor, double rate)
{
  tenors.push_back(tenor);
  vols.push_back(rate);
}

double VolCurve::getVol(Date tenor) const
{
  // TODO: implement interpolation if needed
  if (tenors.empty())
    return 0.0;
  return vols.back();
}

void VolCurve::display() const
{
  cout << "Vol curve: " << name << endl;
  for (size_t i = 0; i < tenors.size(); ++i)
  {
    cout << tenors[i] << ": " << vols[i] << endl;
  }
}

void Market::Print() const
{
  cout << endl << "market asof: " << asOf << endl;

  for (auto curve : curves)
  {                         // first is the key of the dictionary
    curve.second.display(); // second is the value of the dictionary
  }
  for (auto vol : vols)
  {
    vol.second.display();
  }
  cout << endl << "Bond Prices:" << endl;
  for (const auto &bond : bondPrices)
  {
    cout << bond.first << ": " << bond.second << endl;
  }
  
  cout << endl << "Stock Prices:" << endl;
  for (const auto &stock : stockPrices)
  {
    cout << stock.first << ": " << stock.second << endl;
  }
}

void Market::addCurve(const std::string &curveName, const RateCurve &curve)
{
  curves.emplace(curveName, curve);
}

void Market::addVolCurve(const std::string &volCurveName, const VolCurve &vol)
{
  vols.emplace(volCurveName, vol);
}

void Market::addBondPrice(const std::string &bondName, double price)
{
  bondPrices[bondName] = price;
}

void Market::addStockPrice(const std::string &stockName, double price)
{
  stockPrices[stockName] = price;
}

std::ostream &operator<<(std::ostream &os, const Market &mkt)
{
  os << mkt.asOf << std::endl;
  return os;
}

std::istream &operator>>(std::istream &is, Market &mkt)
{
  is >> mkt.asOf;
  return is;
}
