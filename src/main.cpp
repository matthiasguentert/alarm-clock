#include <Arduino.h>
#include <Fsm.h>

#define A_SECOND 1000
#define A_MINUTE 60000

#include "Debug.hpp"
#include "Display.hpp"
#include "Clock.hpp"
#include "Player.hpp"
#include "StateManager.hpp"
#include "Encoder.hpp"
#include "EventHandler.hpp"

const PlayerConfig playerConfig = {
    .volumeLevel = 14, // 0 (min) - 30 (max)
    .softwareSerialRxPin = 5,
    .softwareSerialTxPin = 6};

const DisplayConfig displayConfig = {
    .brighthness = 5}; // 0 (min) - 15 (max)

const ClockConfig clockConfig = {
    .interruptPin = 2, // INT0
    .snoozeButtonPin = 14,
    .snoozeButtonLedPin = 15,
    .alarmSwitchPin = 16,
    .alarmSwitchLedPin = 17};

const AlarmConfig alarmConfig = {
#ifdef DEBUG
    .snoozeDurationMs = 10 * A_SECOND
#else
    .snoozeDurationMs = 5 * A_MINUTE
#endif
};

const EncoderConfig encoderConfig = {
    .addr = 0x36,
    .pin = 24,
    .debounceIntervalMs = 5};

void setup()
{
    D_SerialBeginAndWait(9600);

    StateManager::setup(alarmConfig);
    Player::setup(playerConfig);
    Display::setup(displayConfig);
    Clock::setup(clockConfig);
    Encoder::setup(encoderConfig);

    D_println(">> Startup complete");
}

void loop()
{
    StateManager::fsm.run_machine();

    static unsigned long pressStartTime = 0;
    static bool isLongPress = false;

    Clock::snoozeButton.update();

    if (Clock::snoozeButton.fell())
    {
        pressStartTime = millis();
        isLongPress = false;
    }

    if (Clock::snoozeButton.read() == LOW && !isLongPress)
    {
        if (millis() - pressStartTime > 5 * A_SECOND)
        {
            isLongPress = true;
            D_println(">> long press");
            StateManager::fsm.trigger(TIME_SETUP_EVENT);
        }
    }

    if (Clock::isAlarmEnabled() && Clock::alarmFired())
    {
        StateManager::fsm.trigger(ALARM_EVENT);
    }

    if (Encoder::pressed())
    {
        StateManager::fsm.trigger(ALARM_TIME_SETUP_EVENT);
    }
}
