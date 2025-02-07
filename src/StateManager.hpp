#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#define IDLE_EVENT 1
#define ALARM_TIME_SETUP_EVENT 2
#define ALARM_SOUND_SETUP_EVENT 3
#define ALARM_EVENT 4
#define SNOOZE_EVENT 5
#define TIME_SETUP_EVENT 6

#include <Debug.hpp>
#include <EventHandler.hpp>
#include <Clock.hpp>

#include <Fsm.h>

struct AlarmConfig
{
    uint32_t snoozeDurationMs;
};

class StateManager
{
public:
    static Fsm fsm;

public:
    static void setup(const AlarmConfig &config);

private:
    static State idle;
    static State alarm;
    static State snooze;
    static State alarmTimeSetup;
    static State alarmSoundSetup;
    static State timeSetup;
};

#endif // STATEMANAGER_HPP
