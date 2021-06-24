#include "french_republican_calendar.h"
#include "const.h"
#include <Arduino.h>

// Last full month
const unsigned int FrenchRepublicanCalendar::FRUCTIDOR = 12;
// Leap day
const unsigned int FrenchRepublicanCalendar::REVOLUTION = 18;

// The font used by the watch has been modified to support French characters using the following mapping:
// ! → é
// " → è
// # → ê
// $ → ô
// % → â
// & → ë
// ( → ï
// ) → û
// * → œ
// { → É
// | → Â
// } → Œ
// ' → ’
const char *FrenchRepublicanCalendar::MONTH_NAMES[] = {
    "Vend!miaire", "Brumaire",  "Frimaire",  "Niv$se", "Pluvi$se", "Vent$se", "Germinal", "Flor!al",     "Prairial",
    "Messidor",    "Thermidor", "Fructidor", "Vertu",  "G!nie",    "Travail", "Opinion",  "R!compenses", "R!volution"};

const char *FrenchRepublicanCalendar::DAY_NAMES[] = {"Primidi", "Duodi",   "Tridi",  "Quartidi", "Quintidi",
                                                     "Sextidi", "Septidi", "Octidi", "Nonidi",   "D!cadi"};

FrenchRepublicanCalendar::FrenchRepublicanCalendar() : year(1), monthNumber(1), dayInMonth(1)
{
}

void FrenchRepublicanCalendar::update(const time_t &currentTime)
{
    // January 1st, 1970 (start of the Unix epoch) was the 11 Nivôve of year 178 (using the Romme method)
    unsigned int year = 178;
    unsigned int month = 4;
    unsigned int absoluteDays = 11 + (int)currentTime / (60 * 60 * 24);

#ifdef DEBUG
    Serial.printf("Total absolute days: %d\n", absoluteDays);
#endif

    // Full years
    while (absoluteDays >= this->daysInFrenchYear(year)) {
        absoluteDays -= this->daysInFrenchYear(year);
        year++;
    }

    this->year = year;

#ifdef DEBUG
    Serial.printf("Absolute days after removing full years: %d\n", absoluteDays);
#endif

    // Full Months
    while (absoluteDays >= this->daysInFrenchMonth(year, month)) {
        absoluteDays -= this->daysInFrenchMonth(year, month);
        month++;
    }
    this->monthNumber = month;

#ifdef DEBUG
    Serial.printf("Absolute days after removing full months (and the day of the French month): %d\n", absoluteDays);
#endif

    this->dayInMonth = absoluteDays;
}

bool FrenchRepublicanCalendar::isFrenchLeapYear(unsigned int year) const
{
    // We only need to care about years after 1970
    // Use the Romme method
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int FrenchRepublicanCalendar::daysInFrenchYear(unsigned int year) const
{
    return isFrenchLeapYear(year) ? 366 : 365;
}

int FrenchRepublicanCalendar::daysInFrenchMonth(unsigned int year, unsigned int month) const
{
    if (month <= this->FRUCTIDOR) {
        return 30;
    } else if (month == this->REVOLUTION) {
        return isFrenchLeapYear(year) ? 1 : 0;
    } else {
        return 1;
    }
}

const char *FrenchRepublicanCalendar::getMonthName() const
{
    if (this->monthNumber < 1 || this->monthNumber > sizeof(this->MONTH_NAMES)) {
        return "";
    }

    return this->MONTH_NAMES[this->monthNumber - 1];
}

unsigned int FrenchRepublicanCalendar::getDay() const
{
    return this->dayInMonth;
}

const char *FrenchRepublicanCalendar::getDayName() const
{
    return this->DAY_NAMES[this->dayInMonth % 10 - 1];
}

bool FrenchRepublicanCalendar::sansculottides() const
{
    return this->monthNumber > FRUCTIDOR;
}

unsigned int FrenchRepublicanCalendar::getYear() const
{
    return this->year;
}

const String FrenchRepublicanCalendar::ROMAN_NUMBER_MATRIX[14][2] = {
    {"1000", "M"}, {"900", "CM"}, {"500", "D"}, {"400", "CD"}, {"100", "C"}, {"90", "XC"}, {"50", "L"},
    {"40", "XL"},  {"10", "X"},   {"9", "IX"},  {"5", "V"},    {"4", "IV"},  {"1", "I"},   {"0", ""}};

unsigned int FrenchRepublicanCalendar::floorRoman(unsigned int number) const
{
    unsigned int index = 0;

    while (index < sizeof(this->ROMAN_NUMBER_MATRIX)) {
        if (this->ROMAN_NUMBER_MATRIX[index][0].toInt() <= number) {
            break;
        }
        index++;
    }

    return index;
}

const String FrenchRepublicanCalendar::getRomanizedYear() const
{
    unsigned int year = this->year;
    unsigned int index = 0;
    String output = "";

    while (this->floorRoman(year) < 13) {
        index = this->floorRoman(year);
        output += this->ROMAN_NUMBER_MATRIX[index][1];
        year -= this->ROMAN_NUMBER_MATRIX[index][0].toInt();
    }

    return output;
}
