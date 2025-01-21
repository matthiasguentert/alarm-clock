#include "Player.hpp"

void Player::setup(const PlayerConfig &config)
{
    Player::config = config;

    // Initialize software serial
    serial = SoftwareSerial(config.softwareSerialTxPin, config.softwareSerialRxPin);
    serial.begin(9600);

    // Send initialization parameters
    executeCommand(0x3f, 0x00, 0x00);

    setVolume(config.volumeLevel);
}

void Player::play(Track track)
{
    uint8_t t = static_cast<uint8_t>(track);

    setVolume(config.volumeLevel);
    executeCommand(0x08, highByte(t), lowByte(t));
}

void Player::stop()
{
    executeCommand(0x16, 0x00, 0x00);

    // A hacky way to reduce background noise when idling
    setVolume(0);
    reset();
}

void Player::setVolume(uint8_t level)
{
    executeCommand(0x06, 0x00, level);
}

SoftwareSerial Player::serial(0, 1);

PlayerConfig Player::config;

void Player::executeCommand(uint8_t command, uint8_t param1, uint8_t param2)
{
    // Based on a very shitty documentation...
    uint8_t buffer[] = {
        0x7e,    // 0: start
        0xff,    // 1: version
        0x06,    // 2: length
        command, // 3: command
        0x00,    // 4: feedback
        param1,  // 5: param1
        param2,  // 6: param2  => which track should be played?
        0xff,    // 7: checksum high byte
        0xff,    // 8: checksum low byte
        0xef     // 9: end
    };

    calculateChecksum(buffer);

    // Delay between commands should be > 20ms
    delay(30);
    serial.write(buffer, 10);
    delay(30);
}

void Player::calculateChecksum(uint8_t *buffer)
{
    uint16_t checksum = 0;

    // Checksum: -(version + len + command + feedback + param1 + param2)
    for (uint8_t i = 1; i < 7; i++)
    {
        checksum += buffer[i];
    }
    checksum = -checksum;
    buffer[7] = highByte(checksum);
    buffer[8] = lowByte(checksum);
}

void Player::reset()
{
    executeCommand(0x0c, 0x00, 0x00);
}