#include <iostream>
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
using Poco::DateTime;
using Poco::DateTimeFormat;
int main(int argc, char** argv)
{
	DateTime now;
	std::cout << "Today is "
		<< DateTimeFormat::WEEKDAY_NAMES[now.dayOfWeek()] << ", "
		<< DateTimeFormat::MONTH_NAMES[now.month() - 1] << " "
		<< now.day() << " - "
		<< "day number "
		<< now.dayOfYear()
		<< " in "
		<< now.year()
		<< " and day number "
		<< (long)now.julianDay()
		<< " in the Julian calendar." << std::endl;
	return 0;
}