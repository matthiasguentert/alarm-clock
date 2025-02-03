#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Debug.hpp>
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

struct DisplayConfig
{
    uint8_t brighthness;
};

class Display
{
public:
    static void setup(const DisplayConfig &config);
    static void update();
    static void update(DateTime now);
    static void update(uint8_t hour, uint8_t minute);
    static void writeDigit(uint8_t position, uint8_t number);
    static void clear();
    static void clear(uint8_t position);

public:
    static void startBlinking();
    static void stopBlinking();

private:
    static Adafruit_7segment matrix;
    static bool colonToggle;
};

#endif // DISPLAY_HPP
