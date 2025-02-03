#include "Display.hpp"

Adafruit_7segment Display::matrix;
bool Display::colonToggle;

void Display::setup(const DisplayConfig &config)
{
    D_println(">> Display setup");

    matrix.begin(0x70);
    matrix.drawColon(true);
    matrix.setBrightness(config.brighthness);

    colonToggle = false;
}

void Display::update()
{
    matrix.writeDisplay();
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

void Display::update(uint8_t hour, uint8_t minute)
{
    matrix.writeDigitNum(0, hour / 10);
    matrix.writeDigitNum(1, hour % 10);
    matrix.writeDigitNum(3, minute / 10);
    matrix.writeDigitNum(4, minute % 10);

    matrix.writeDisplay();
}

void Display::clear()
{
    matrix.clear();
}

void Display::clear(uint8_t position)
{
    matrix.writeDigitRaw(position, 0);
}

void Display::writeDigit(uint8_t position, uint8_t number)
{
    matrix.writeDigitNum(position, number);
}

void Display::startBlinking()
{
    matrix.blinkRate(HT16K33_BLINK_1HZ);
}

void Display::stopBlinking()
{
    matrix.blinkRate(HT16K33_BLINK_OFF);
}
