#ifndef REVOLUTION_H
#define REVOLUTION_H

#include "french_republican_calendar.h"
#include "decimal_time.h"
#include <Watchy.h>

class Revolution : public Watchy
{
public:
    enum class Mode
    {
        DigitalDate,
        Digital,
        AnalogDate,
        Analog
    };

private:
    FrenchRepublicanCalendar calendar;
    DecimalTime decimalTime;
    const bool darkMode;
    const float_t handWidth;
    Mode mode;

public:
    Revolution(bool darkMode, float_t handWidth);
    void drawWatchFace() override;
    // Reimplemented from Watchy to use ALARM1 instead of ALARM2
    void init(String datetime = "");
    // Reimplemented from Watchy to use ALARM1 instead of ALARM2
    void handleButtonPress();

private:
    void drawDigitalTime();
    void drawAnalogTime();
    void drawDate();
    void drawCenteredString(const char *str, const int x, const int y);
    void resetAlarm();
    // Reimplemented from Watchy to use ALARM1 instead of ALARM2
    void _rtcConfig(String datetime);
};

#endif
