#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <Debug.hpp>
#include <Display.hpp>
#include <Player.hpp>
#include <StateManager.hpp>
#include <RTClib.h>
#include <Bounce2.h>

struct ClockConfig
{
    uint8_t interruptPin;
    uint8_t snoozeButtonPin;
    uint8_t snoozeButtonLedPin;
    uint8_t alarmSwitchPin;
    uint8_t alarmSwitchLedPin;
};

class Clock
{
public:
    static void setup(const ClockConfig &config);
    static bool alarmFired();
    static bool isAlarmEnabled();
    static DateTime getAlarmTime();
    static DateTime getTime();
    static void setAlarmTime(uint8_t hour, uint8_t minute);
    static void setTime(uint8_t hour, uint8_t minute);
    static String toString(DateTime dt);

public:
    static void onIdleEnter();
    static void onIdle();
    static void onIdleExit();
    static void onAlarmEnter();
    static void onAlarm();
    static void onAlarmExit();
    static void onSnoozeEnter();
    static void onSnoozeExit();

public:
    static Bounce2::Button snoozeButton;

private:
    static RTC_DS3231 rtc;
    static volatile bool isrToggle;
    static ClockConfig config;

private:
    static void onClockInterrupt();
};

#endif // CLOCK_HPP
