#pragma once

#include <string>

using namespace std;

class Date
{
public:
	Date() {};

	Date(const string& dateStr);
	int operator-(const Date& dt2);

private:
	int year = 1900;
	int month = 1;
	int day = 1;

};

// return number of days between 2 dates object
int Date::operator-(const Date& dt2) {
	int numOfYs = dt2.year - this->year;
	int numOfMs = dt2.month - this->month;
	int numOfDs = dt2.day - this->day;
	return numOfYs * 365 + numOfMs * 30 + numOfDs;
}