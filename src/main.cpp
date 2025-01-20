#include <Arduino.h>
#include <Fsm.h>

#define DEBUG 1
#include "Debug.hpp"
#include "Display.hpp"
#include "Clock.hpp"
#include "Player.hpp"

PlayerConfig playerConfig = {
    .volumeLevel = 14,
    .softwareSerialRxPin = 5,
    .softwareSerialTxPin = 6};

DisplayConfig displayConfig = {
    .brighthness = 3 // 0 (min) - 15 (max)
};

ClockConfig clockConfig = {
    .interruptPin = 2 // INT0
};

State idle(nullptr, Clock::onIdle, nullptr);
State alarm(Clock::onAlarmEnter, Clock::onAlarm, Clock::onAlarmExit);
State snooze(nullptr, nullptr, nullptr);     // not implemented yet
State timeSetup(nullptr, nullptr, nullptr);  // not implemented yet
State alarmSetup(nullptr, nullptr, nullptr); // not implemented yet

Fsm fsm(&idle); // show the current time as initial state

void setup()
{
    // State* state_from, State* state_to, int event, void (*on_transition)()
    fsm.add_transition(&idle, &alarm, 1, nullptr);      // alarm fired
    fsm.add_transition(&alarm, &idle, 2, nullptr);      // alarm has been turned off
    fsm.add_transition(&alarm, &snooze, 3, nullptr);    // snooze state
    fsm.add_transition(&snooze, &alarm, 4, nullptr);    // snooze timer expired, back to alarm
    fsm.add_transition(&snooze, &idle, 5, nullptr);     // from snooze back to idle
    fsm.add_transition(&idle, &alarmSetup, 6, nullptr); // alarm setup
    fsm.add_transition(&alarmSetup, &idle, 7, nullptr); // finished alarm setup
    fsm.add_transition(&idle, &timeSetup, 8, nullptr);  // set the current time
    fsm.add_transition(&timeSetup, &idle, 9, nullptr);  // finished setting the time

    Player::setup(playerConfig);
    Display::setup(displayConfig);
    Clock::setup(clockConfig);
}

void loop()
{
    fsm.run_machine();
}
