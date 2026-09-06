#include "PhysicalRandom.h"

// The whole implementation is AVR-only (see PhysicalRandom.h). Guarding it here too means this
// file compiles to nothing on other architectures, instead of failing on the AVR-only registers
// and headers below (the Arduino build compiles every .cpp in src/ regardless of what a sketch
// actually includes), so this file is built even for projects that only use the rest of the library.
#if defined(__AVR__)

#include <avr/wdt.h>

/**
 * How many physical sampling rounds (8 raw bits each) are folded into each output byte. Two
 * rounds per byte means 16 raw bits go into every 8 bits of output.
 */
#define ENTROPY_ROUNDS_PER_BYTE 2

static volatile uint8_t entropySample = 0;
static volatile bool entropySampleReady = false;
static uint8_t savedTccr1a = 0;
static uint8_t savedTccr1b = 0;

/**
 * Fires every 16 ms, clocked by the watchdog's own oscillator.
 *
 * TCNT1L is the low byte of Timer1, which is counting on the crystal. Because the two clocks
 * drift against each other, the moment this interrupt is entered is not fixed relative to that
 * count (so its low bit is unpredictable).
 *
 * This claims the watchdog interrupt vector for the whole program. A sketch that defines its own
 * ISR(WDT_vect) will fail to link.
 */
ISR(WDT_vect)
{
    entropySample = TCNT1L;
    entropySampleReady = true;

    // In interrupt-only mode the hardware clears WDIE once the interrupt runs, so it has to be
    // re-armed for the next period. Setting WDIE alone needs no timed sequence.
    WDTCSR |= (1 << WDIE);
}

/** Takes over Timer1 and the watchdog. Must be paired with `endEntropySampling()`. */
static void beginEntropySampling()
{
    // Timer1 is what the interrupt samples. The Arduino core leaves it in phase-correct PWM mode
    // (that is what analogWrite uses on pins 9 and 10), counting at 250 kHz. Free-running at the
    // full 16 MHz gives a much faster-moving counter, and so cleaner jitter. Saved and restored
    // so PWM still works afterwards.
    savedTccr1a = TCCR1A;
    savedTccr1b = TCCR1B;
    TCCR1A = 0;
    TCCR1B = (1 << CS10);

    cli();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    // Changing WDE or the prescaler requires this timed sequence: set WDCE, then write within
    // four clock cycles.
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    // WDIE alone: fire an interrupt, never reset the chip. All WDP bits clear: 16 ms period.
    WDTCSR = (1 << WDIE);
    sei();
}

static void endEntropySampling()
{
    cli();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0;
    sei();

    TCCR1B = savedTccr1b;
    TCCR1A = savedTccr1a;
}

/** Blocks until the next watchdog tick - roughly 16 ms - and returns one raw bit. */
static uint8_t nextEntropyBit()
{
    entropySampleReady = false;
    while (!entropySampleReady)
    {
    }
    return entropySample & 0x01;
}

void physicalRandom(uint8_t* buffer, uint8_t length)
{
    if (length == 0)
    {
        return;
    }

    beginEntropySampling();

    uint32_t hash = 2166136261UL; // FNV-1a offset basis.

    // Two nearly worthless sources, folded in early because they are free. They cannot make the
    // result less random, and they are the only thing left differing between boards if the
    // watchdog trick ever fails on some future chip.
    hash = (hash ^ (uint8_t)analogRead(A0)) * 16777619UL;
    hash = (hash ^ (uint8_t)micros()) * 16777619UL;

    for (uint8_t byteIndex = 0; byteIndex < length; byteIndex++)
    {
        for (uint8_t round = 0; round < ENTROPY_ROUNDS_PER_BYTE; round++)
        {
            uint8_t collected = 0;
            for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
            {
                collected = (collected << 1) | nextEntropyBit();
            }
            hash = (hash ^ collected) * 16777619UL;
        }

        // Each byte is carved off the hash as it stands after its own rounds, then folding
        // continues for the next byte - so every output byte reflects fresh physical samples
        // rather than being derived from the same 32 bits of entropy repeated.
        buffer[byteIndex] = (uint8_t)hash;
    }

    endEntropySampling();
}

uint32_t physicalRandom32()
{
    uint8_t bytes[4];
    physicalRandom(bytes, sizeof(bytes));

    uint32_t value = 0;
    for (uint8_t i = 0; i < sizeof(bytes); i++)
    {
        value = (value << 8) | bytes[i];
    }

    return value;
}

uint64_t physicalRandom64()
{
    uint8_t bytes[8];
    physicalRandom(bytes, sizeof(bytes));

    uint64_t value = 0;
    for (uint8_t i = 0; i < sizeof(bytes); i++)
    {
        value = (value << 8) | bytes[i];
    }

    return value;
}

#endif // defined(__AVR__)
