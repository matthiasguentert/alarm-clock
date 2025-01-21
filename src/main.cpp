#include <Arduino.h>
#include <Fsm.h>

#define DEBUG 1

#include "Debug.hpp"
#include "Display.hpp"
#include "Clock.hpp"
#include "Player.hpp"
#include "StateManager.hpp"

PlayerConfig playerConfig = {
    .volumeLevel = 14, // 0 (min) - 30 (max)
    .softwareSerialRxPin = 5,
    .softwareSerialTxPin = 6};

DisplayConfig displayConfig = {
    .brighthness = 3 // 0 (min) - 15 (max)
};

ClockConfig clockConfig = {
    .interruptPin = 2 // INT0
};

void setup()
{
    StateManager::setup();
    Player::setup(playerConfig);
    Display::setup(displayConfig);
    Clock::setup(clockConfig);
}

void loop()
{
    StateManager::fsm.run_machine();
}
