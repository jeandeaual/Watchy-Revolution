#ifndef FRENCH_REPUBLICAN_CALENDAR_H
#define FRENCH_REPUBLICAN_CALENDAR_H

#include <time.h>
#include <Arduino.h>

class FrenchRepublicanCalendar
{
private:
    static const unsigned int FRUCTIDOR;
    static const unsigned int REVOLUTION;
    static const char *MONTH_NAMES[];
    static const char *DAY_NAMES[];
    static const String ROMAN_NUMBER_MATRIX[14][2];
    unsigned int year;
    unsigned int monthNumber;
    unsigned int dayInMonth;

public:
    FrenchRepublicanCalendar();
    void update(const time_t &currentTime);
    unsigned int getYear() const;
    const String getRomanizedYear() const;
    const char *getMonthName() const;
    unsigned int getDay() const;
    const char *getDayName() const;
    bool sansculottides() const;

private:
    bool isFrenchLeapYear(unsigned int year) const;
    int daysInFrenchYear(unsigned int year) const;
    int daysInFrenchMonth(unsigned int year, unsigned int month) const;
    unsigned int floorRoman(unsigned int number) const;
};

#endif