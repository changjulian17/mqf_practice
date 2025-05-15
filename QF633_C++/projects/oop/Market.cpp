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
