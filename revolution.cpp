#include "revolution.h"
#include "fonts/LibertinusSerif_Regular_French_ASCII16pt7b.h"
#include "fonts/LibertinusSerif_Regular_Numbers42pt7b.h"
#include <Arduino.h>

Revolution::Revolution()
{
}

void Revolution::drawWatchFace()
{
    this->drawTime();
    this->drawDate();
}

void Revolution::drawTime()
{
    const unsigned int hours = this->decimalTime.getHours();
    const unsigned int minutes = this->decimalTime.getMinutes();
    char time[5];

    time[0] = '0' + hours % 10;
    time[1] = ':';
    time[2] = '0' + minutes / 10 % 10;
    time[3] = '0' + minutes % 10;
    time[4] = '\0';

    this->display.setFont(&LibertinusSerif_Regular_Numbers42pt7b);
    this->display.setTextWrap(false);

    this->drawCenteredString(time, GxEPD2_154_D67::WIDTH / 2, GxEPD2_154_D67::HEIGHT / 2 - 35);
}

void Revolution::drawDate()
{
    const uint16_t x = GxEPD2_154_D67::WIDTH / 2;
    const uint16_t y_offset = 28;
    const uint16_t base_y = GxEPD2_154_D67::HEIGHT / 2;

    this->display.setFont(&LibertinusSerif_Regular_French_ASCII16pt7b);
    this->display.setTextWrap(false);

    const char *dayOfWeek = this->calendar.getWeekDayName();
    int day = this->calendar.getDay();
    const char *month = this->calendar.getMonthName();
    const String year = this->calendar.getRomanizedYear();
    const char *dayOfYear = this->calendar.getYearDayName(FrenchRepublicanCalendar::Language::French);
    char *date;

    if (this->calendar.sansculottides()) {
        asprintf(&date, "%s", month);
    } else {
        asprintf(&date, "%d %s", day, month);
    }

    this->drawCenteredString(dayOfWeek, x, base_y);
    this->drawCenteredString(date, x, base_y + y_offset);
    this->drawCenteredString(year.c_str(), x, base_y + y_offset * 2);
    this->drawCenteredString(dayOfYear, x, base_y + y_offset * 3);

    free(date);
}

void Revolution::drawCenteredString(const char *str, const int x, const int y)
{
    int16_t x1, y1;
    uint16_t w, h;

    this->display.getTextBounds(str, x, y, &x1, &y1, &w, &h);
    this->display.setCursor(x - w / 2, y);
    this->display.print(str);
}

// Reimplemented from Watchy to use ALARM1 instead of ALARM2
void Revolution::init(String datetime)
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    Wire.begin(SDA, SCL);

    switch (wakeup_reason) {
#ifdef ESP_RTC
    case ESP_SLEEP_WAKEUP_TIMER: // ESP Internal RTC
        if (guiState == WATCHFACE_STATE) {
            this->RTC.read(this->currentTime);
            this->currentTime.Minute++;
            tmElements_t tm;
            tm.Month = this->currentTime.Month;
            tm.Day = this->currentTime.Day;
            tm.Year = this->currentTime.Year;
            tm.Hour = this->currentTime.Hour;
            tm.Minute = this->currentTime.Minute;
            tm.Second = 0;
            time_t t = makeTime(tm);
            this->RTC.set(t);
            this->RTC.read(this->currentTime);
            this->showWatchFace(true); // Partial updates on tick
        }
        break;
#endif
    case ESP_SLEEP_WAKEUP_EXT0:   // RTC Alarm
        this->RTC.alarm(ALARM_1); // Reset the alarm flag in the RTC
        if (guiState == WATCHFACE_STATE) {
            this->RTC.read(this->currentTime);
            this->calendar.update(makeTime(this->currentTime));
            this->decimalTime.update(this->currentTime);
            this->RTC.setAlarm(ALM1_MATCH_MINUTES, this->decimalTime.getNextAlarmWakeSeconds(),
                               this->decimalTime.getNextAlarmWakeMinutes(), 0, 0);
            this->showWatchFace(true); // Partial updates on tick
        }
        break;
    case ESP_SLEEP_WAKEUP_EXT1: // Button press
        this->handleButtonPress();
        break;
    default: // Reset
#ifndef ESP_RTC
        this->_rtcConfig(datetime);
#endif
        this->calendar.update(makeTime(this->currentTime));
        this->decimalTime.update(this->currentTime);
        this->RTC.setAlarm(ALM1_MATCH_MINUTES, this->decimalTime.getNextAlarmWakeSeconds(),
                           this->decimalTime.getNextAlarmWakeMinutes(), 0, 0);

        // Skip the BMA423 configuration since we won't use it
        this->showWatchFace(false); // Full update on reset
        break;
    }

    deepSleep();
}

// Reimplemented from Watchy to use ALARM1 instead of ALARM2
void Revolution::handleButtonPress()
{
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if (wakeupBit & MENU_BTN_MASK) {
        // Menu button
        if (guiState == WATCHFACE_STATE) {
            // Enter menu state if coming from watch face
            showMenu(menuIndex, false);
        } else if (guiState == MAIN_MENU_STATE) {
            // If already in menu, then select menu item
            switch (menuIndex) {
            case 0:
                this->showBattery();
                break;
            case 1:
                this->showBuzz();
                break;
            case 2:
                this->showAccelerometer();
                break;
            case 3:
                this->setTime();
                break;
            case 4:
                this->setupWifi();
                break;
            case 5:
                this->showUpdateFW();
                break;
            default:
                break;
            }
        } else if (guiState == FW_UPDATE_STATE) {
            this->updateFWBegin();
        }
    } else if (wakeupBit & BACK_BTN_MASK) {
        // Back button
        if (guiState == MAIN_MENU_STATE) {
            // Exit to watch face if already in menu
            // Resets the alarm flag in the RTC
            this->RTC.alarm(ALARM_1);
            this->RTC.read(this->currentTime);
            this->calendar.update(makeTime(this->currentTime));
            this->decimalTime.update(this->currentTime);
            this->RTC.setAlarm(ALM1_MATCH_MINUTES, this->decimalTime.getNextAlarmWakeSeconds(),
                               this->decimalTime.getNextAlarmWakeMinutes(), 0, 0);
            this->showWatchFace(false);
        } else if (guiState == APP_STATE) {
            // Exit to menu if already in app~
            this->showMenu(menuIndex, false);
        } else if (guiState == FW_UPDATE_STATE) {
            // Exit to menu if already in app
            this->showMenu(menuIndex, false);
        }
    } else if (wakeupBit & UP_BTN_MASK) {
        // Up button
        if (guiState == MAIN_MENU_STATE) {
            // Decrement menu index
            menuIndex--;
            if (menuIndex < 0) {
                menuIndex = MENU_LENGTH - 1;
            }
            this->showMenu(menuIndex, true);
        }
    }
    // Down Button
    else if (wakeupBit & DOWN_BTN_MASK) {
        if (guiState == MAIN_MENU_STATE) {
            // Increment menu index
            menuIndex++;
            if (menuIndex > MENU_LENGTH - 1) {
                menuIndex = 0;
            }
            this->showMenu(menuIndex, true);
        }
    }

    /***************** Fast menu *****************/
    bool timeout = false;
    long lastTimeout = millis();
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);
    while (!timeout) {
        if (millis() - lastTimeout > 5000) {
            timeout = true;
        } else {
            if (digitalRead(MENU_BTN_PIN) == 1) {
                lastTimeout = millis();
                if (guiState == MAIN_MENU_STATE) {
                    // If already in menu, then select menu item
                    switch (menuIndex) {
                    case 0:
                        this->showBattery();
                        break;
                    case 1:
                        this->showBuzz();
                        break;
                    case 2:
                        this->showAccelerometer();
                        break;
                    case 3:
                        this->setTime();
                        break;
                    case 4:
                        this->setupWifi();
                        break;
                    case 5:
                        this->showUpdateFW();
                        break;
                    default:
                        break;
                    }
                } else if (guiState == FW_UPDATE_STATE) {
                    this->updateFWBegin();
                }
            } else if (digitalRead(BACK_BTN_PIN) == 1) {
                lastTimeout = millis();
                if (guiState == MAIN_MENU_STATE) {
                    // Exit to watch face if already in menu
                    // Resets the alarm flag in the RTC
                    this->RTC.alarm(ALARM_1);
                    this->RTC.read(this->currentTime);
                    this->calendar.update(makeTime(this->currentTime));
                    this->decimalTime.update(this->currentTime);
                    this->RTC.setAlarm(ALM1_MATCH_MINUTES, this->decimalTime.getNextAlarmWakeSeconds(),
                                       this->decimalTime.getNextAlarmWakeMinutes(), 0, 0);
                    this->showWatchFace(false);
                    // Leave loop
                    break;
                } else if (guiState == APP_STATE) {
                    // Exit to menu if already in app
                    this->showMenu(menuIndex, false);
                } else if (guiState == FW_UPDATE_STATE) {
                    // Exit to menu if already in app
                    this->showMenu(menuIndex, false);
                }
            } else if (digitalRead(UP_BTN_PIN) == 1) {
                lastTimeout = millis();
                if (guiState == MAIN_MENU_STATE) {
                    // Decrement menu index
                    menuIndex--;
                    if (menuIndex < 0) {
                        menuIndex = MENU_LENGTH - 1;
                    }
                    this->showFastMenu(menuIndex);
                }
            } else if (digitalRead(DOWN_BTN_PIN) == 1) {
                lastTimeout = millis();
                if (guiState == MAIN_MENU_STATE) {
                    // Increment menu index
                    menuIndex++;
                    if (menuIndex > MENU_LENGTH - 1) {
                        menuIndex = 0;
                    }
                    this->showFastMenu(menuIndex);
                }
            }
        }
    }

    display.hibernate();
}

// Reimplemented from Watchy to use ALARM1 instead of ALARM2
void Revolution::_rtcConfig(String datetime)
{
    // https://github.com/JChristensen/DS3232RTC
    this->RTC.squareWave(SQWAVE_NONE); // disable square wave output
    // Alarm wakes up Watchy every second
    this->RTC.setAlarm(ALM1_EVERY_SECOND, 0, 0, 0, 0);
    // Enable alarm interrupt
    this->RTC.alarmInterrupt(ALARM_1, true);

    this->RTC.read(this->currentTime);
}
