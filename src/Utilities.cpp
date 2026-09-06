#include "Utilities.h"

/** Reads a single hexadecimal digit. Returns false if `character` isn't one. */
static bool hexNibble(char character, uint8_t& nibble)
{
    if (character >= '0' && character <= '9')
    {
        nibble = character - '0';
        return true;
    }
    if (character >= 'A' && character <= 'F')
    {
        nibble = character - 'A' + 10;
        return true;
    }
    if (character >= 'a' && character <= 'f')
    {
        nibble = character - 'a' + 10;
        return true;
    }

    return false;
}

void bytesToHex(const uint8_t* buffer, uint8_t length, char* destination)
{
    for (uint8_t i = 0; i < length; i++)
    {
        const uint8_t highNibble = (buffer[i] >> 4) & 0x0F;
        const uint8_t lowNibble = buffer[i] & 0x0F;

        destination[i * 2] = highNibble < 10 ? ('0' + highNibble) : ('A' + highNibble - 10);
        destination[i * 2 + 1] = lowNibble < 10 ? ('0' + lowNibble) : ('A' + lowNibble - 10);
    }

    destination[length * 2] = '\0';
}

bool hexToBytes(const char* text, uint8_t* buffer, uint8_t length)
{
    if (text == NULL)
    {
        return false;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        uint8_t highNibble, lowNibble;
        if (!hexNibble(text[i * 2], highNibble) || !hexNibble(text[i * 2 + 1], lowNibble))
        {
            return false;
        }

        buffer[i] = (highNibble << 4) | lowNibble;
    }

    return text[length * 2] == '\0';
}

void bytes32ToHex(uint32_t value, char* destination)
{
    uint8_t bytes[4];
    for (uint8_t i = 0; i < sizeof(bytes); i++)
    {
        bytes[i] = (uint8_t)(value >> (8 * (sizeof(bytes) - 1 - i)));
    }

    bytesToHex(bytes, sizeof(bytes), destination);
}

uint32_t hexToBytes32(const char* text)
{
    uint8_t bytes[4];
    if (!hexToBytes(text, bytes, sizeof(bytes)))
    {
        return 0;
    }

    uint32_t value = 0;
    for (uint8_t i = 0; i < sizeof(bytes); i++)
    {
        value = (value << 8) | bytes[i];
    }

    return value;
}

void bytes64ToHex(uint64_t value, char* destination)
{
    uint8_t bytes[8];
    for (uint8_t i = 0; i < sizeof(bytes); i++)
    {
        bytes[i] = (uint8_t)(value >> (8 * (sizeof(bytes) - 1 - i)));
    }

    bytesToHex(bytes, sizeof(bytes), destination);
}

uint64_t hexToBytes64(const char* text)
{
    uint8_t bytes[8];
    if (!hexToBytes(text, bytes, sizeof(bytes)))
    {
        return 0;
    }

    uint64_t value = 0;
    for (uint8_t i = 0; i < sizeof(bytes); i++)
    {
        value = (value << 8) | bytes[i];
    }

    return value;
}
