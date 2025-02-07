#include "StateManager.hpp"

// State definitions
State StateManager::idle(Clock::onIdleEnter, Clock::onIdle, Clock::onIdleExit);
State StateManager::alarm(Clock::onAlarmEnter, Clock::onAlarm, Clock::onAlarmExit);
State StateManager::snooze(Clock::onSnoozeEnter, Clock::onIdle, Clock::onSnoozeExit);
State StateManager::timeSetup(EventHandler::onTimeSetupEnter, EventHandler::onTimeSetup, EventHandler::onTimeSetupExit);
State StateManager::alarmTimeSetup(EventHandler::onAlarmTimeSetupEnter, EventHandler::onAlarmTimeSetup, EventHandler::onAlarmTimeSetupExit);
State StateManager::alarmSoundSetup(nullptr, EventHandler::onAlarmSoundSetup, nullptr);

Fsm StateManager::fsm(&idle);

void StateManager::setup(const AlarmConfig &config)
{
    D_println(">> StateManager setup");

    // Transition definitions
    fsm.add_transition(&idle, &alarm, ALARM_EVENT, nullptr);
    fsm.add_transition(&alarm, &idle, IDLE_EVENT, nullptr);
    fsm.add_transition(&alarm, &snooze, SNOOZE_EVENT, nullptr);
    fsm.add_timed_transition(&snooze, &alarm, config.snoozeDurationMs, nullptr);
    fsm.add_transition(&snooze, &idle, IDLE_EVENT, nullptr);

    fsm.add_transition(&idle, &alarmTimeSetup, ALARM_TIME_SETUP_EVENT, nullptr);
    fsm.add_transition(&alarmTimeSetup, &alarmSoundSetup, ALARM_SOUND_SETUP_EVENT, nullptr);
    fsm.add_transition(&alarmSoundSetup, &idle, IDLE_EVENT, nullptr);

    fsm.add_transition(&idle, &timeSetup, TIME_SETUP_EVENT, nullptr);
    fsm.add_transition(&timeSetup, &idle, IDLE_EVENT, nullptr);
}
