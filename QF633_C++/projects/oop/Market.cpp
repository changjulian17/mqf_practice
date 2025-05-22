#include "Market.h"
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

vector<pair<string, string>> readKeyValueFile(const string& fileName) {
    vector<pair<string, string>> result;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos == string::npos) continue;
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        // Trim leading/trailing whitespace
        key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);

        // Remove % and \r
        value.erase(remove(value.begin(), value.end(), '%'), value.end());
        value.erase(remove(value.begin(), value.end(), '\r'), value.end());

        result.emplace_back(key, value);
    }
    return result;
}

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
  if (tenorDates.empty())
    return 0.0;

  // Exact match
  for (size_t i = 0; i < tenorDates.size(); ++i) {
    if (tenorDates[i] == tenor)
      return rates[i];
  }

  // If before first tenor, return first rate
  if (tenor < tenorDates.front())
    return rates.front();

  // If after last tenor, return last rate
  if (tenor > tenorDates.back())
    return rates.back();

  // Linear interpolation between two tenors
  for (size_t i = 1; i < tenorDates.size(); ++i) {
    if (tenor < tenorDates[i]) {
      double t1 = tenorDates[i-1].toDouble();
      double t2 = tenorDates[i].toDouble();
      double r1 = rates[i-1];
      double r2 = rates[i];
      double t = tenor.toDouble();
      return r1 + (r2 - r1) * (t - t1) / (t2 - t1);
    }
  }
  return 0;
}

void VolCurve::addVol(Date tenor, double rate)
{
  tenors.push_back(tenor);
  vols.push_back(rate);
}

double VolCurve::getVol(Date tenor) const
{
    if (tenors.empty()) { return 0.0;}

    // Exact match
    for (size_t i = 0; i < tenors.size(); ++i) {
        if (tenors[i] == tenor){
            cout << "Exact match vol for tenor " << tenors[i] << ": " << vols[i] << endl;
            return vols[i];}
    }

    // If before first tenor, return first vol
    if (tenor < tenors.front()){
        return vols.front();}

    // If after last tenor, return last vol
    if (tenor > tenors.back()){
        return vols.back();}

    // Linear interpolation between two tenors
    for (size_t i = 1; i < tenors.size(); ++i) {
        if (tenor < tenors[i]) {
            double t1 = tenors[i-1].toDouble();
            double t2 = tenors[i].toDouble();
            double v1 = vols[i-1];
            double v2 = vols[i];
            double t = tenor.toDouble();
            return v1 + (v2 - v1) * (t - t1) / (t2 - t1);
        }
    }
    return 0.0; // fallback, should not reach here
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

double RateCurve::getMarketSwapRate(Date startDate, Date endDate, int frequency) const
{
    double years = endDate.toDouble() - startDate.toDouble();
    int nPeriods = static_cast<int>(round(frequency * years));
    double dt = 1.0 / frequency;
    double denom = 0.0;

    for (int i = 1; i <= nPeriods; ++i) {
        // Compute payDate = startDate + i * (12 / frequency) months
        int monthsToAdd = (12 / frequency) * i;
        Date payDate = startDate;
        payDate.month += monthsToAdd;
        while (payDate.month > 12) {
            payDate.year += 1;
            payDate.month -= 12;
        }

        double t = payDate.toDouble() - startDate.toDouble();
        double z = getRate(payDate);
        double df = exp(-z * t);
        denom += dt * df;
    }

    // Compute D(start) and D(end)
    double tStart = startDate.toDouble() - startDate.toDouble(); // = 0
    double tEnd = endDate.toDouble() - startDate.toDouble();
    double zStart = getRate(startDate);
    double zEnd = getRate(endDate);
    double dfStart = exp(-zStart * tStart); // = 1
    double dfEnd = exp(-zEnd * tEnd);

    return (dfStart - dfEnd) / denom;
}
