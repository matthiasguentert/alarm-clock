#define DEBUG 0

#include "Debug.hpp"
#include "Clock.hpp"
#include "Display.hpp"
#include "Player.hpp"
#include "StateManager.hpp"

void Clock::setup(const ClockConfig &config)
{
    if (!rtc.begin())
    {
        D_println("RTC initialization failed");
        while (1)
            ;
    };

    // We don't need the 32K Pin, so disable it
    rtc.disable32K();

    // Configure the RTC to output a 1 Hz square wave on the INT/SQW pin
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

    // Attach the interrupt to the pin connected to the DS3231 SQW pin
    pinMode(config.interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(config.interruptPin), Clock::onClockInterrupt, FALLING);

    // Configure alarm switch
    pinMode(config.alarmSwitchPin, INPUT_PULLUP);
    pinMode(config.alarmSwitchLedPin, OUTPUT); // 100 ohm, green LED, pretty dazzlying
}

void Clock::onIdle()
{
    if (isrToggle)
    {
        isrToggle = false;
        Display::update(rtc.now());
    }
}

void Clock::onAlarmEnter()
{
    Player::play(Track::FUNK);
}

void Clock::onAlarm()
{
    snoozeButton.update();

    if (snoozeButton.fell())
    {
        StateManager::fsm.trigger(SNOOZE_EVENT);
    }

    // blink LEDs
    // if alaram swich off trigger idle event
}

void Clock::onAlarmExit()
{
    Player::stop();
}

RTC_DS3231 Clock::rtc;

volatile bool Clock::isrToggle = false;

Bounce2::Button Clock::snoozeButton;

void Clock::onClockInterrupt()
{
    isrToggle = true;
}