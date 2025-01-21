#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#define ALARM_EVENT 1
#define IDLE_EVENT 2
#define SNOOZE_EVENT 3
#define ALARM_SETUP_EVENT 4
#define TIME_SETUP_EVENT 5

#include <Fsm.h>

// Thin layer around the FSM object
class StateManager
{
public:
    static Fsm fsm;
    static void setup();

private:
};

#endif // STATEMANAGER_HPP