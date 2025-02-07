#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Debug.hpp>
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

#define NUM_TRACKS 8

class Player
{
public:
    static void setup(const PlayerConfig &config);

public:
    static void play();
    static void play(Track track);
    static void stop();
    static void setVolume(uint8_t level);

public:
    static Track toAlarmTrack(uint8_t alarmTrack);
    static uint8_t fromAlarmTrack(Track alarmTrack);

public:
    static Track readAlarmTrackFromEeprom();
    static void writeAlarmTrackToEeprom(Track alarmTrack);

private:
    static SoftwareSerial serial;
    static PlayerConfig config;

private:
    static void executeCommand(uint8_t command, uint8_t param1, uint8_t param2);
    static void calculateChecksum(uint8_t *buffer);
    static void reset();
};

#endif // PLAYER_HPP
