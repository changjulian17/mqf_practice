#ifndef DATE_H 			// macro to prevent multiple or redundant inclusions
#define DATE_H

#include <iostream>

class Date
{
public:
	int year = 0;
	int month = 0;
	int day = 0;
	Date(int y, int m, int d) : year(y), month(m), day(d) {};
	Date() {
		std::cout << "default Date constructor called" << std::endl;
	};
};

double operator-(const Date& d1, const Date& d2);
std::ostream& operator<<(std::ostream& os, const Date& date); 		// when overloading "<<" here is referring only to the ostream one"
std::istream& operator>>(std::istream& is, Date& date);

#endif
