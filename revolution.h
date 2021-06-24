#ifndef REVOLUTION_H
#define REVOLUTION_H

#include "french_republican_calendar.h"
#include "decimal_time.h"
#include <Watchy.h>

class Revolution : public Watchy
{
private:
    FrenchRepublicanCalendar calendar;
    DecimalTime decimalTime;

public:
    Revolution();
    void drawWatchFace() override;
    // Reimplemented from Watchy to use ALARM1 instead of ALARM2
    void init(String datetime = "");
    // Reimplemented from Watchy to use ALARM1 instead of ALARM2
    void handleButtonPress();

private:
    void drawTime();
    void drawDate();
    void drawCenteredString(const char *str, const int x, const int y);
    // Reimplemented from Watchy to use ALARM1 instead of ALARM2
    void _rtcConfig(String datetime);
};

#endif
