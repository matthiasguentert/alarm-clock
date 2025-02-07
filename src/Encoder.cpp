#include "Encoder.hpp"

int32_t Encoder::encoderValue;
EncoderConfig Encoder::config;
Adafruit_seesaw Encoder::ss;
bool Encoder::lastButtonState;
uint64_t Encoder::lastDebounceTime;
bool Encoder::buttonState;
bool Encoder::buttonPressed;

void Encoder::setup(const EncoderConfig &config)
{
    D_println(">> Encoder setup");

    Encoder::config = config;
    Encoder::encoderValue = 0;

    while (!ss.begin(config.addr))
    {
        D_println(">> Encoder initializing...");
        delay(10);
    };

    ss.pinMode(config.pin, INPUT_PULLUP);
}

// returns the encoder value in a cyclic fashion
int32_t Encoder::getPosition(int32_t maxValue)
{
    uint32_t delta = ss.getEncoderDelta();
    encoderValue += delta;

    if (encoderValue < 0)
    {
        encoderValue = (encoderValue + maxValue) % (maxValue);
    }
    else if (encoderValue > maxValue - 1)
    {
        encoderValue = encoderValue % (maxValue);
    }

    return encoderValue;
}

void Encoder::setPosition(int32_t position)
{
    encoderValue = position;
    ss.setEncoderPosition(position);
}

bool Encoder::pressed()
{
    // Read the state of the push button
    bool reading = ss.digitalRead(config.pin);

    if (reading != lastButtonState)
    {
        lastDebounceTime = millis(); // Reset the debounce timer
    }

    // If the button state has remained stable for longer than the debounce delay
    if ((millis() - lastDebounceTime) > config.debounceIntervalMs)
    {
        // If the button state has changed, update the buttonState
        if (reading != buttonState)
        {
            buttonState = reading;

            // Check if the new button state is LOW (button pressed)
            if (buttonState == LOW)
            {
                buttonPressed = true;
                return true;
            }
        }
    }

    lastButtonState = reading;

    return false;
}
