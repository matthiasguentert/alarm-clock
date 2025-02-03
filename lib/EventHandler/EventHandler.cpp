#include "EventHandler.hpp"

void EventHandler::onAlarmTimeSetupEnter()
{
    D_println(">> Alarm time setup enter");

    Display::startBlinking();
}

void EventHandler::onAlarmTimeSetup()
{
    D_println(">> Alarm time setup");

    // Reading current alarm time and displaying it
    DateTime currentAlarm = Clock::getAlarmTime();
    Display::update(currentAlarm.hour(), currentAlarm.minute());
    D_print(">> Current alarm time: ");
    D_println(Clock::toString(currentAlarm));

    // Extract hour and minute from current alarm time
    uint8_t alarmHour = currentAlarm.hour();
    uint8_t alarmMinute = currentAlarm.minute();

    // Set encoder to current alarm hour
    Encoder::setPosition(alarmHour);
    while (!Encoder::pressed())
    {
        // Read encoder position
        alarmHour = Encoder::getPosition(24);

        // Update display
        Display::writeDigit(0, alarmHour / 10);
        Display::writeDigit(1, alarmHour % 10);

        // Clear minutes
        Display::clear(3);
        Display::clear(4);

        Display::update();
    }

    // Set encoder to current alarm minute
    Encoder::setPosition(alarmMinute);
    while (!Encoder::pressed())
    {
        // Read encoder position
        alarmMinute = Encoder::getPosition(60);

        // Update display
        Display::writeDigit(3, alarmMinute / 10);
        Display::writeDigit(4, alarmMinute % 10);

        // Clear hours
        Display::clear(0);
        Display::clear(1);

        Display::update();
    }

    // Storing alarm time
    D_print(">> Setting alarm time to: ");
    D_println(Clock::toString(DateTime(2017, 06, 23, alarmHour, alarmMinute, 0)));
    Clock::setAlarmTime(alarmHour, alarmMinute);

    StateManager::fsm.trigger(ALARM_SOUND_SETUP_EVENT);
}

void EventHandler::onAlarmTimeSetupExit()
{
    D_println(">> Alarm time setup exit");

    Display::stopBlinking();
}

void EventHandler::onAlarmSoundSetup()
{
    D_println(">> Alarm sound setup");

    D_print(">> Reading current alarm track from EEPROM: ");
    Track currentSelection = Player::readAlarmTrackFromEeprom();
    D_println(Player::fromAlarmTrack(currentSelection));

    // Set encoder position to current alarm track
    Encoder::setPosition(Player::fromAlarmTrack(currentSelection) - 1);
    Display::clear();

    // Play selected track
    Player::play(currentSelection);

    while (!Encoder::pressed())
    {
        uint8_t selection = Encoder::getPosition(NUM_TRACKS) + 1;

        Display::writeDigit(4, selection);
        Display::update();

        Track selectedTrack = Player::toAlarmTrack(selection);

        // Play selected track only if selection changes
        if (selectedTrack != currentSelection)
        {
            Player::play(selectedTrack);
            currentSelection = selectedTrack;
        }
    }

    D_print(">> Writing alarm track selection to EEPROM: ");
    D_println(Player::fromAlarmTrack(currentSelection));
    Player::writeAlarmTrackToEeprom(currentSelection);

    Player::stop();

    StateManager::fsm.trigger(IDLE_EVENT);
}

void EventHandler::onTimeSetupEnter()
{
    D_println(">> Time setup enter");

    Display::startBlinking();
}

void EventHandler::onTimeSetup()
{
    D_println(">> Time setup");

    // Reading current time and displaying it
    DateTime currentTime = Clock::getTime();
    Display::update(currentTime.hour(), currentTime.minute());
    D_print(">> Current time: ");
    D_println(Clock::toString(currentTime));

    uint8_t hour = currentTime.hour();
    uint8_t minute = currentTime.minute();

    Encoder::setPosition(hour);
    while (!Encoder::pressed())
    {
        hour = Encoder::getPosition(24);

        Display::writeDigit(0, hour / 10);
        Display::writeDigit(1, hour % 10);

        Display::clear(3);
        Display::clear(4);

        Display::update();
    }

    Encoder::setPosition(minute);
    while (!Encoder::pressed())
    {
        minute = Encoder::getPosition(60);

        Display::writeDigit(3, minute / 10);
        Display::writeDigit(4, minute % 10);

        Display::update();
    }

    D_print(">> Setting time to: ");
    D_println(Clock::toString(DateTime(2017, 06, 23, hour, minute, 0)));
    Clock::setTime(hour, minute);

    StateManager::fsm.trigger(IDLE_EVENT);
}

void EventHandler::onTimeSetupExit()
{
    D_println(">> Time setup exit");

    Display::stopBlinking();
}
