#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <RTClib.h>

struct DisplayConfig
{
    uint8_t brighthness;
};

class Display
{
public:
    static void setup(const DisplayConfig &config);
    static void update(DateTime now);

private:
    static Adafruit_7segment matrix;
    static bool colonToggle;
};

#endif // DISPLAY_HPP