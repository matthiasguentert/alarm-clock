#define DEBUG 0
#include "Debug.hpp"
#include "Display.hpp"

void Display::setup(const DisplayConfig &config)
{
    matrix.begin(0x70);
    matrix.drawColon(true);
    matrix.setBrightness(config.brighthness);

    colonToggle = false;
}

void Display::update(DateTime now)
{
    colonToggle = !colonToggle;
    matrix.drawColon(colonToggle);

    matrix.writeDigitNum(0, now.hour() / 10);
    matrix.writeDigitNum(1, now.hour() % 10);
    matrix.writeDigitNum(3, now.minute() / 10);
    matrix.writeDigitNum(4, now.minute() % 10);

    matrix.writeDisplay();
}

Adafruit_7segment Display::matrix;

bool Display::colonToggle;