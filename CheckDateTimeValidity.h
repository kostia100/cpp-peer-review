#pragma once

#include <stdexcept>
#include <array>
#include <vector>

using namespace std::literals;


enum class Units {
    YEAR,
    MONTH,
    DAY,
    HOUR,
    MINUTE,
    SECONDE
};

struct DateTime {

	// maybe other stuff...

	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

    void CheckDateTimeValidity() const {
        CheckValueInRange("year", year, MAX_YEAR, MIN_YEAR);
        CheckValueInRange("month", month, MAX_MONTH, MIN_MONTH);
        CheckValueInRange("day",day, MaxMonthDay(), MIN_DAY);
        CheckValueInRange("hour",hour,MAX_HOUR,MIN_HOUR);
        CheckValueInRange("minute", minute, MAX_MIN, MIN_MIN);
        CheckValueInRange("second", second, MAX_SEC, MIN_SEC);
    }

private: 

    static void CheckValueInRange(std::string name, int val, int max, int min) {
        if (val < min) {
            throw std::domain_error(name + " is too small"s);
        }
        if (val > max) {
            throw std::domain_error(name + " is too big"s);
        }
    }

    bool IsLeapYear() const {
        return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
    }

    int MaxMonthDay() const {
        const int MAX_DAY[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int february = 1;
        return month == february ? MAX_DAY[month+IsLeapYear()] : MAX_DAY[month];
    }



    const static int MAX_SEC = 59;
    const static int MIN_SEC = 0;

    const static int MAX_MIN = 59;
    const static int MIN_MIN = 0;

    const static int MAX_HOUR = 23;
    const static int MIN_HOUR = 0;

    const static int MAX_MONTH = 12;
    const static int MIN_MONTH = 1;

    const static int MIN_DAY = 1;

    const static int MAX_YEAR = 9999;
    const static int MIN_YEAR = 1;
    

};

