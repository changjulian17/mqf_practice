#include "Date.h"
#include <string>
#include <cctype>

//return date difference in fraction of year
double operator-(const Date& d1, const Date& d2)
{
	int yearDiff = d1.year - d2.year;
	int monthDiff = (d1.month - d2.month);
	int dayDiff = d1.day - d2.day;
	return yearDiff + monthDiff / 12.0 + dayDiff / 365.0;
}

Date Date::addTenor(const Date& base, const std::string& tenor) {
    Date d = base;
    if (tenor == "ON") {
        d.day += 1;
    } else if (tenor.back() == 'M') {
        int months = std::stoi(tenor.substr(0, tenor.size() - 1));
        d.month += months;
        while (d.month > 12) {
            d.year += 1;
            d.month -= 12;
        }
    } else if (tenor.back() == 'Y') {
        int years = std::stoi(tenor.substr(0, tenor.size() - 1));
        d.year += years;
    }
    return d;
}

Date operator+(const Date& base, const std::string& tenor) {
    return Date::addTenor(base, tenor);
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	os << d.year << "-" << d.month << "-" << d.day << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, Date& d)
{
	is >> d.year >> d.month >> d.day;
	return is;
}
