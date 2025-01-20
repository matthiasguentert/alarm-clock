#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Arduino.h>
#include <SoftwareSerial.h>

struct PlayerConfig
{
    uint8_t volumeLevel;
    uint8_t softwareSerialRxPin;
    uint8_t softwareSerialTxPin;
};

enum class Track : uint8_t
{
    DRUMS = 1,
    FUNK = 2,
    MEOW = 3,
    BARKING = 4,
    ROOSTER = 5,
    CUCKOO = 6,
    MELODY = 7,
    BEEPBEEP = 8
};

class Player
{
public:
    static void setup(const PlayerConfig &config);
    static void play(Track track);
    static void stop();
    static void setVolume(uint8_t level);

private:
    static SoftwareSerial serial;
    static PlayerConfig config;

    static void executeCommand(uint8_t command, uint8_t param1, uint8_t param2);
    static void calculateChecksum(uint8_t *buffer);
    static void reset();
};

#endif // PLAYER_HPP
