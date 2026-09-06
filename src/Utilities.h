#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

/**
 * Writes a buffer of bytes as uppercase hexadecimal text, most significant byte first.
 * @param buffer The bytes to convert.
 * @param length How many bytes `buffer` holds.
 * @param destination The buffer to write into. Must hold at least `length` * 2 + 1 bytes.
 *
 * Example:
 * ```cpp
 * uint8_t bytes[4] = {0x1A, 0x03, 0xF2, 0x9C};
 * char text[9]; // length * 2 + 1
 * bytesToHex(bytes, sizeof(bytes), text);
 * // text now holds "1A03F29C"
 * ```
 */
void bytesToHex(const uint8_t* buffer, uint8_t length, char* destination);

/**
 * Reads a buffer of bytes back from its hexadecimal text form. Case-insensitive.
 * @param text Exactly `length` * 2 hexadecimal digits, null-terminated.
 * @param buffer The buffer to fill. Must hold at least `length` bytes.
 * @param length How many bytes to read.
 * @returns Returns true if `text` was valid and `buffer` was filled, false otherwise.
 *
 * Example:
 * ```cpp
 * uint8_t bytes[4];
 * bool success = hexToBytes("1A03F29C", bytes, sizeof(bytes));
 * // success is true, bytes now holds {0x1A, 0x03, 0xF2, 0x9C}
 * ```
 */
bool hexToBytes(const char* text, uint8_t* buffer, uint8_t length);

/**
 * Wrapper around `bytesToHex()` for a 32-bit value.
 * @param value The value to convert.
 * @param destination The buffer to write into. Must hold at least 9 bytes.
 */
void bytes32ToHex(uint32_t value, char* destination);

/**
 * Wrapper around `hexToBytes()` for a 32-bit value.
 * @param text Exactly 8 hexadecimal digits, null-terminated.
 * @returns Returns the 32-bit value, or 0 if `text` is not valid.
 */
uint32_t hexToBytes32(const char* text);

/**
 * Wrapper around `bytesToHex()` for a 64-bit value.
 * @param value The value to convert.
 * @param destination The buffer to write into. Must hold at least 17 bytes.
 */
void bytes64ToHex(uint64_t value, char* destination);

/**
 * Wrapper around `hexToBytes()` for a 64-bit value.
 * @param text Exactly 16 hexadecimal digits, null-terminated.
 * @returns Returns the 64-bit value, or 0 if `text` is not valid.
 */
uint64_t hexToBytes64(const char* text);

#endif
