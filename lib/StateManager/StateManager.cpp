#include "StateManager.hpp"
#include "Clock.hpp"

Fsm StateManager::fsm;

void StateManager::setup()
{
    // State definitions
    State idle(nullptr, Clock::onIdle, nullptr);
    State alarm(Clock::onAlarmEnter, Clock::onAlarm, Clock::onAlarmExit);
    State snooze(nullptr, nullptr, nullptr);     // not implemented yet
    State timeSetup(nullptr, nullptr, nullptr);  // not implemented yet
    State alarmSetup(nullptr, nullptr, nullptr); // not implemented yet

    fsm = Fsm(&idle);

    // Transition definitions
    fsm.add_transition(&idle, &alarm, ALARM_EVENT, nullptr);            // alarm fired
    fsm.add_transition(&alarm, &idle, IDLE_EVENT, nullptr);             // alarm has been turned off
    fsm.add_transition(&alarm, &snooze, SNOOZE_EVENT, nullptr);         // snooze state
    fsm.add_transition(&snooze, &alarm, ALARM_EVENT, nullptr);          // snooze timer expired, back to alarm
    fsm.add_transition(&snooze, &idle, IDLE_EVENT, nullptr);            // from snooze back to idle
    fsm.add_transition(&idle, &alarmSetup, ALARM_SETUP_EVENT, nullptr); // alarm setup
    fsm.add_transition(&alarmSetup, &idle, IDLE_EVENT, nullptr);        // finished alarm setup
    fsm.add_transition(&idle, &timeSetup, TIME_SETUP_EVENT, nullptr);   // set the current time
    fsm.add_transition(&timeSetup, &idle, IDLE_EVENT, nullptr);         // finished setting the time
}