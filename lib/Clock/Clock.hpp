#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <RTClib.h>
#include <Bounce2.h>

struct ClockConfig
{
    uint8_t interruptPin;
    uint8_t alarmSwitchPin;
    uint8_t alarmSwitchLedPin;
};

class Clock
{
public:
    static void setup(const ClockConfig &config);
    static void onIdle();
    static void onAlarmEnter();
    static void onAlarm();
    static void onAlarmExit();

private:
    static RTC_DS3231 rtc;
    static volatile bool isrToggle;
    static Bounce2::Button snoozeButton;

    static void onClockInterrupt();
};

#endif // CLOCK_HPP