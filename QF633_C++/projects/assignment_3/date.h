#pragma once

#include <string>
#include <cmath>

using namespace std;

class Date {
public:
    Date() {}

    // Constructor to parse a date string in the format "YYYY-MM-DD"
    Date(const string& dateStr);

    // Overload "-" operator to compute the year fraction between two dates
    double operator-(const Date& dt2) const;

    // Getter methods for year, month, and day
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
    
private:
    int year = 1900;
    int month = 1;
    int day = 1;

    // Helper function to compute the total number of days since year 0
    int toDays() const;
};