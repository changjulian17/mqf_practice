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

	static Date addTenor(const Date& base, const std::string& tenor); // Add this line

	// Convert date to fractional year (e.g., 2025.375 for May 2025)
	double toDouble() const;

	bool operator==(const Date& other) const;
	bool operator<(const Date& other) const;
	bool operator>(const Date& other) const;
};

double operator-(const Date& d1, const Date& d2);

std::ostream& operator<<(std::ostream& os, const Date& date); 		// when overloading "<<" here is referring only to the ostream one"
std::istream& operator>>(std::istream& is, Date& date);

Date operator+(const Date& base, const std::string& tenor);

#endif
