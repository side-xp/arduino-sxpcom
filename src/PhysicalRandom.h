#ifndef PHYSICALRANDOM_H
#define PHYSICALRANDOM_H

#include <Arduino.h>

#if defined(__AVR__)

/**
 * Fills a buffer with random bytes, measured from the physical jitter between the watchdog timer's
 * own oscillator and Timer1's crystal-clocked count. Two independent, physically distinct
 * oscillators drifting against each other is what makes each sampled bit unpredictable and
 * different per chip, since no two oscillators run at exactly the same rate.
 *
 * This is AVR-only: it takes over the watchdog timer and Timer1 registers directly. On other
 * architectures, use a random source appropriate to that board instead (e.g. a factory-programmed
 * unique ID, or a hardware RNG peripheral where available).
 *
 * Blocks for roughly 256 ms per requested byte (about 1 second for 4 bytes) while it collects raw
 * entropy, so call it sparingly. A good use for this is generating a value in initialization and
 * use it as a seed of `randomSeed()` function if you need more and cheaper random cheaply at
 * runtime.
 *
 * @param buffer The buffer to fill. Must hold at least `length` bytes.
 * @param length How many random bytes to generate.
 */
void physicalRandom(uint8_t* buffer, uint8_t length);

/**
 * Wrapper around `physicalRandom()` for a 32-bit value.
 * @returns A random 32-bit value.
 */
uint32_t physicalRandom32();

/**
 * Wrapper around `physicalRandom()` for a 64-bit value.
 * @returns A random 64-bit value.
 */
uint64_t physicalRandom64();

#else
#warning "PhysicalRandom is AVR-only and is not available on this board. Use a random source " \
         "appropriate to this architecture instead (e.g. a factory-programmed unique ID, or a " \
         "hardware RNG peripheral where available)."
#endif

#endif
