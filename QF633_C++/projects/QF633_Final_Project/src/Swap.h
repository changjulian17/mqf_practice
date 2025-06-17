#pragma once
#include "Trade.h"

class Swap : public Trade {
public:
	//make necessary change
	Swap(string name, Date start, Date end, double _notional, double _rate, double _freq)
	{
		tradeType = "Swap";
		underlying = name;
		startDate = start;
		maturityDate = end;
		tradeDate = start;
		notional = _notional;
		tradeRate = _rate;
		frequency = _freq;
		generateSwapSchedule();
	}

	/*
	implement this, using npv = discounted cash flow from both leg;
	*/
	inline string getType() const { return tradeType; };
	inline string getUnderlying() const { return underlying; };
	double Payoff(double r) const;
	double Pv(const Market& mkt) const;
	double getAnnuity(const Market& mkt) const; //implement this in a cpp file
	void generateSwapSchedule();

private:
	Date startDate;
	Date maturityDate;
	double tradeRate; // fixed leg rate
	double frequency; // use 1 for annual, 2 for semi-annual etc
	vector<Date> swapSchedule;
	string rateCurve;

};