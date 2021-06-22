#include "revolution.h"
#include "fonts/LibertinusSerif_Regular39pt7b.h"

Revolution::Revolution() {}

void Revolution::drawWatchFace()
{
    const unsigned int hours = this->decimal_minutes / 100;
    const unsigned int minutes = this->decimal_minutes % 100;
    const int16_t x = GxEPD2_154_D67::WIDTH / 2;
    const int16_t y = GxEPD2_154_D67::HEIGHT / 2;
    int16_t x1, y1;
    uint16_t w, h;

    char time[5];
    time[0] = '0' + hours % 10;
    time[1] = ':';
    time[2] = '0' + minutes / 10 % 10;
    time[3] = '0' + minutes % 10;
    time[4] = '\0';

    this->display.setFont(&LibertinusSerif_Regular39pt7b);
    this->display.setTextWrap(false);
    this->display.getTextBounds(time, x, y, &x1, &y1, &w, &h);
    this->display.setCursor(x - w / 2, y + 20);
    this->display.print(time);
}

// Calculate the current decimal time and set a new alarm
void Revolution::computeDecimalTime()
{
    this->RTC.read(this->currentTime);

    // Rounded decimal minute steps in sexagesimal seconds since no milliseconds are available from the DS3232RTC
    // for (unsigned int i = 0; i < 1000; i++) { steps[i] = round(i * 86.4); }
    static const unsigned int steps[] = {
        86,    173,   259,   346,   432,   518,   605,   691,   778,   864,   950,   1037,  1123,  1210,  1296,  1382,
        1469,  1555,  1642,  1728,  1814,  1901,  1987,  2074,  2160,  2246,  2333,  2419,  2506,  2592,  2678,  2765,
        2851,  2938,  3024,  3110,  3197,  3283,  3370,  3456,  3542,  3629,  3715,  3802,  3888,  3974,  4061,  4147,
        4234,  4320,  4406,  4493,  4579,  4666,  4752,  4838,  4925,  5011,  5098,  5184,  5270,  5357,  5443,  5530,
        5616,  5702,  5789,  5875,  5962,  6048,  6134,  6221,  6307,  6394,  6480,  6566,  6653,  6739,  6826,  6912,
        6998,  7085,  7171,  7258,  7344,  7430,  7517,  7603,  7690,  7776,  7862,  7949,  8035,  8122,  8208,  8294,
        8381,  8467,  8554,  8640,  8726,  8813,  8899,  8986,  9072,  9158,  9245,  9331,  9418,  9504,  9590,  9677,
        9763,  9850,  9936,  10022, 10109, 10195, 10282, 10368, 10454, 10541, 10627, 10714, 10800, 10886, 10973, 11059,
        11146, 11232, 11318, 11405, 11491, 11578, 11664, 11750, 11837, 11923, 12010, 12096, 12182, 12269, 12355, 12442,
        12528, 12614, 12701, 12787, 12874, 12960, 13046, 13133, 13219, 13306, 13392, 13478, 13565, 13651, 13738, 13824,
        13910, 13997, 14083, 14170, 14256, 14342, 14429, 14515, 14602, 14688, 14774, 14861, 14947, 15034, 15120, 15206,
        15293, 15379, 15466, 15552, 15638, 15725, 15811, 15898, 15984, 16070, 16157, 16243, 16330, 16416, 16502, 16589,
        16675, 16762, 16848, 16934, 17021, 17107, 17194, 17280, 17366, 17453, 17539, 17626, 17712, 17798, 17885, 17971,
        18058, 18144, 18230, 18317, 18403, 18490, 18576, 18662, 18749, 18835, 18922, 19008, 19094, 19181, 19267, 19354,
        19440, 19526, 19613, 19699, 19786, 19872, 19958, 20045, 20131, 20218, 20304, 20390, 20477, 20563, 20650, 20736,
        20822, 20909, 20995, 21082, 21168, 21254, 21341, 21427, 21514, 21600, 21686, 21773, 21859, 21946, 22032, 22118,
        22205, 22291, 22378, 22464, 22550, 22637, 22723, 22810, 22896, 22982, 23069, 23155, 23242, 23328, 23414, 23501,
        23587, 23674, 23760, 23846, 23933, 24019, 24106, 24192, 24278, 24365, 24451, 24538, 24624, 24710, 24797, 24883,
        24970, 25056, 25142, 25229, 25315, 25402, 25488, 25574, 25661, 25747, 25834, 25920, 26006, 26093, 26179, 26266,
        26352, 26438, 26525, 26611, 26698, 26784, 26870, 26957, 27043, 27130, 27216, 27302, 27389, 27475, 27562, 27648,
        27734, 27821, 27907, 27994, 28080, 28166, 28253, 28339, 28426, 28512, 28598, 28685, 28771, 28858, 28944, 29030,
        29117, 29203, 29290, 29376, 29462, 29549, 29635, 29722, 29808, 29894, 29981, 30067, 30154, 30240, 30326, 30413,
        30499, 30586, 30672, 30758, 30845, 30931, 31018, 31104, 31190, 31277, 31363, 31450, 31536, 31622, 31709, 31795,
        31882, 31968, 32054, 32141, 32227, 32314, 32400, 32486, 32573, 32659, 32746, 32832, 32918, 33005, 33091, 33178,
        33264, 33350, 33437, 33523, 33610, 33696, 33782, 33869, 33955, 34042, 34128, 34214, 34301, 34387, 34474, 34560,
        34646, 34733, 34819, 34906, 34992, 35078, 35165, 35251, 35338, 35424, 35510, 35597, 35683, 35770, 35856, 35942,
        36029, 36115, 36202, 36288, 36374, 36461, 36547, 36634, 36720, 36806, 36893, 36979, 37066, 37152, 37238, 37325,
        37411, 37498, 37584, 37670, 37757, 37843, 37930, 38016, 38102, 38189, 38275, 38362, 38448, 38534, 38621, 38707,
        38794, 38880, 38966, 39053, 39139, 39226, 39312, 39398, 39485, 39571, 39658, 39744, 39830, 39917, 40003, 40090,
        40176, 40262, 40349, 40435, 40522, 40608, 40694, 40781, 40867, 40954, 41040, 41126, 41213, 41299, 41386, 41472,
        41558, 41645, 41731, 41818, 41904, 41990, 42077, 42163, 42250, 42336, 42422, 42509, 42595, 42682, 42768, 42854,
        42941, 43027, 43114, 43200, 43286, 43373, 43459, 43546, 43632, 43718, 43805, 43891, 43978, 44064, 44150, 44237,
        44323, 44410, 44496, 44582, 44669, 44755, 44842, 44928, 45014, 45101, 45187, 45274, 45360, 45446, 45533, 45619,
        45706, 45792, 45878, 45965, 46051, 46138, 46224, 46310, 46397, 46483, 46570, 46656, 46742, 46829, 46915, 47002,
        47088, 47174, 47261, 47347, 47434, 47520, 47606, 47693, 47779, 47866, 47952, 48038, 48125, 48211, 48298, 48384,
        48470, 48557, 48643, 48730, 48816, 48902, 48989, 49075, 49162, 49248, 49334, 49421, 49507, 49594, 49680, 49766,
        49853, 49939, 50026, 50112, 50198, 50285, 50371, 50458, 50544, 50630, 50717, 50803, 50890, 50976, 51062, 51149,
        51235, 51322, 51408, 51494, 51581, 51667, 51754, 51840, 51926, 52013, 52099, 52186, 52272, 52358, 52445, 52531,
        52618, 52704, 52790, 52877, 52963, 53050, 53136, 53222, 53309, 53395, 53482, 53568, 53654, 53741, 53827, 53914,
        54000, 54086, 54173, 54259, 54346, 54432, 54518, 54605, 54691, 54778, 54864, 54950, 55037, 55123, 55210, 55296,
        55382, 55469, 55555, 55642, 55728, 55814, 55901, 55987, 56074, 56160, 56246, 56333, 56419, 56506, 56592, 56678,
        56765, 56851, 56938, 57024, 57110, 57197, 57283, 57370, 57456, 57542, 57629, 57715, 57802, 57888, 57974, 58061,
        58147, 58234, 58320, 58406, 58493, 58579, 58666, 58752, 58838, 58925, 59011, 59098, 59184, 59270, 59357, 59443,
        59530, 59616, 59702, 59789, 59875, 59962, 60048, 60134, 60221, 60307, 60394, 60480, 60566, 60653, 60739, 60826,
        60912, 60998, 61085, 61171, 61258, 61344, 61430, 61517, 61603, 61690, 61776, 61862, 61949, 62035, 62122, 62208,
        62294, 62381, 62467, 62554, 62640, 62726, 62813, 62899, 62986, 63072, 63158, 63245, 63331, 63418, 63504, 63590,
        63677, 63763, 63850, 63936, 64022, 64109, 64195, 64282, 64368, 64454, 64541, 64627, 64714, 64800, 64886, 64973,
        65059, 65146, 65232, 65318, 65405, 65491, 65578, 65664, 65750, 65837, 65923, 66010, 66096, 66182, 66269, 66355,
        66442, 66528, 66614, 66701, 66787, 66874, 66960, 67046, 67133, 67219, 67306, 67392, 67478, 67565, 67651, 67738,
        67824, 67910, 67997, 68083, 68170, 68256, 68342, 68429, 68515, 68602, 68688, 68774, 68861, 68947, 69034, 69120,
        69206, 69293, 69379, 69466, 69552, 69638, 69725, 69811, 69898, 69984, 70070, 70157, 70243, 70330, 70416, 70502,
        70589, 70675, 70762, 70848, 70934, 71021, 71107, 71194, 71280, 71366, 71453, 71539, 71626, 71712, 71798, 71885,
        71971, 72058, 72144, 72230, 72317, 72403, 72490, 72576, 72662, 72749, 72835, 72922, 73008, 73094, 73181, 73267,
        73354, 73440, 73526, 73613, 73699, 73786, 73872, 73958, 74045, 74131, 74218, 74304, 74390, 74477, 74563, 74650,
        74736, 74822, 74909, 74995, 75082, 75168, 75254, 75341, 75427, 75514, 75600, 75686, 75773, 75859, 75946, 76032,
        76118, 76205, 76291, 76378, 76464, 76550, 76637, 76723, 76810, 76896, 76982, 77069, 77155, 77242, 77328, 77414,
        77501, 77587, 77674, 77760, 77846, 77933, 78019, 78106, 78192, 78278, 78365, 78451, 78538, 78624, 78710, 78797,
        78883, 78970, 79056, 79142, 79229, 79315, 79402, 79488, 79574, 79661, 79747, 79834, 79920, 80006, 80093, 80179,
        80266, 80352, 80438, 80525, 80611, 80698, 80784, 80870, 80957, 81043, 81130, 81216, 81302, 81389, 81475, 81562,
        81648, 81734, 81821, 81907, 81994, 82080, 82166, 82253, 82339, 82426, 82512, 82598, 82685, 82771, 82858, 82944,
        83030, 83117, 83203, 83290, 83376, 83462, 83549, 83635, 83722, 83808, 83894, 83981, 84067, 84154, 84240, 84326,
        84413, 84499, 84586, 84672, 84758, 84845, 84931, 85018, 85104, 85190, 85277, 85363, 85450, 85536, 85622, 85709,
        85795, 85882, 85968, 86054, 86141, 86227, 86314, 86400};

    // Convert the current time to seconds
    int secs_total = (this->currentTime.Hour * 3600) + (this->currentTime.Minute * 60) + this->currentTime.Second;
    int next_step;

    for (unsigned int x = 0; x < 1000; x++) {
        if (secs_total < steps[x]) {
            next_step = steps[x];
            this->decimal_minutes = x;
            break;
        }
    }

    int next_wake_m = floor(next_step / 60 % 60);
    int next_wake_s = floor(next_step % 60);

    this->RTC.setAlarm(ALM1_MATCH_MINUTES, next_wake_s, next_wake_m, 0, 0);
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
            this->computeDecimalTime();
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
        this->computeDecimalTime();
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
            this->computeDecimalTime();
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
                    this->computeDecimalTime();
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
