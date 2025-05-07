#include <cmath>
#include <string>
#include "date.h"

// Constructor to parse a date string in the format "YYYY-MM-DD"
Date::Date(const string& dateStr) {
    year = stoi(dateStr.substr(0, 4));  // Extract year
    month = stoi(dateStr.substr(5, 2)); // Extract month
    day = stoi(dateStr.substr(8, 2));   // Extract day
}

// Helper function to compute the total number of days since year 0
int Date::toDays() const {
    return year * 365 + month * 30 + day; // Simplified calculation
}

// Overload "-" operator to compute the year fraction between two dates
double Date::operator-(const Date& dt2) const {
    int daysInYear = 365; // Simplified; can be adjusted for leap years
    int totalDays1 = this->toDays();
    int totalDays2 = dt2.toDays();
    return static_cast<double>(totalDays1 - totalDays2) / daysInYear;
}