#include <cmath>
#include "date.h"

Date::Date(const string& dateStr) 
{
	year = stoi(dateStr.substr(0, 3));
	// month = stoi(dateStr.substr(0, 3));
	// date = stoi(dateStr.substr(0, 3));


}
