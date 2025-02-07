#include "Clock.hpp"

RTC_DS3231 Clock::rtc;
volatile bool Clock::isrToggle = false;
Bounce2::Button Clock::snoozeButton;
ClockConfig Clock::config;

void Clock::setup(const ClockConfig &config)
{
    D_println(">> Clock setup");

    Clock::config = config;

    if (!rtc.begin())
    {
        D_println(">> RTC initialization failed");
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

    // Configure snooze button with LED
    snoozeButton.attach(config.snoozeButtonPin, INPUT_PULLUP);
    snoozeButton.interval(5);
    snoozeButton.setPressedState(LOW);

    pinMode(config.snoozeButtonLedPin, OUTPUT);   // 47 ohm, orange LED
    digitalWrite(config.snoozeButtonLedPin, LOW); // initially, turn snooze LED off

    // Configure alarm switch with LED
    pinMode(config.alarmSwitchPin, INPUT_PULLUP);
    pinMode(config.alarmSwitchLedPin, OUTPUT);                                               // 100 ohm, green LED, pretty dazzlying
    digitalWrite(config.alarmSwitchLedPin, digitalRead(config.alarmSwitchPin) ? LOW : HIGH); // Set initial LED state based on switch
}

bool Clock::alarmFired()
{
    return rtc.alarmFired(1);
}

bool Clock::isAlarmEnabled()
{
    return !digitalRead(config.alarmSwitchPin);
}

void Clock::onIdleEnter()
{
    D_println(">> idle enter");
}

void Clock::onIdle()
{
    if (isrToggle)
    {
        isrToggle = false;
        Display::update(rtc.now());
    }

    // Update alarm switch LED
    digitalWrite(config.alarmSwitchLedPin, digitalRead(config.alarmSwitchPin) ? LOW : HIGH);
}

void Clock::onIdleExit()
{
    D_println(">> idle leave");
}

void Clock::onAlarmEnter()
{
    D_println(">> starting alarm");

    Player::play();
}

void Clock::onAlarm()
{
    snoozeButton.update();

    // Entering snooze state
    if (snoozeButton.pressed())
    {
        StateManager::fsm.trigger(SNOOZE_EVENT);
    }

    // Alarm got disabled
    if (!isAlarmEnabled())
    {
        rtc.clearAlarm(1);
        StateManager::fsm.trigger(IDLE_EVENT);
    };

    // Update time while in alarm state
    if (isrToggle)
    {
        isrToggle = false;
        Display::update(rtc.now());
    };
}

void Clock::onAlarmExit()
{
    D_println(">> stopping alarm");

    Player::stop();
}

void Clock::onSnoozeEnter()
{
    D_println(">> snooze enter");

    // Turn LED on
    digitalWrite(config.snoozeButtonLedPin, HIGH);
}

void Clock::onSnoozeExit()
{
    D_println(">> snooze leave");

    // Turn LED off
    digitalWrite(config.snoozeButtonLedPin, LOW);
}

DateTime Clock::getAlarmTime()
{
    return rtc.getAlarm1();
}

DateTime Clock::getTime()
{
    return rtc.now();
}

void Clock::setAlarmTime(uint8_t hour, uint8_t minute)
{
    // Required, otherwise setting alarm doesn't work
    rtc.clearAlarm(1);
    rtc.writeSqwPinMode(DS3231_OFF);

    // Alarm when hours, minutes and seconds match
    rtc.setAlarm1(DateTime(2017, 06, 23, hour, minute, 0), DS3231_A1_Hour);
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
}

void Clock::setTime(uint8_t hour, uint8_t minute)
{
    rtc.adjust(DateTime(2017, 06, 23, hour, minute, 0));
}

void Clock::onClockInterrupt()
{
    isrToggle = true;
}

String Clock::toString(DateTime dt)
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", dt.hour(), dt.minute());

    return String(buffer);
}
