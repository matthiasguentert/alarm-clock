#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <Debug.hpp>
#include <Clock.hpp>
#include <Display.hpp>
#include <Encoder.hpp>
#include <StateManager.hpp>
#include <Player.hpp>
#include <RTClib.h>

class EventHandler
{
public:
    static void onAlarmTimeSetupEnter();
    static void onAlarmTimeSetup();
    static void onAlarmTimeSetupExit();
    static void onAlarmSoundSetup();
    static void onTimeSetupEnter();
    static void onTimeSetup();
    static void onTimeSetupExit();

private:
    static String toString(DateTime dt);
};

#endif // EVENTHANDLER_HPP
