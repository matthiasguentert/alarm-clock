#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Debug.hpp>
#include <Adafruit_seesaw.h>

struct EncoderConfig
{
    uint8_t addr;
    uint8_t pin;
    uint8_t debounceIntervalMs;
};

class Encoder
{
public:
    static void setup(const EncoderConfig &config);
    static int32_t getPosition(int32_t maxValue);
    static void setPosition(int32_t position);
    static bool pressed();

private:
    static EncoderConfig config;
    static Adafruit_seesaw ss;
    static bool lastButtonState;
    static uint64_t lastDebounceTime;
    static bool buttonState;
    static bool buttonPressed;
    static int32_t encoderValue;
};

#endif // ENCODER_HPP
