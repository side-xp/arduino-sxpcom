/**
 * Example for seeding Arduino's random() by using the `PhysicalRandom` utility.
 */

#include "PhysicalRandom.h"

void setup()
{
    Serial.begin(9600);

    // Generate a random value using physical noise.
    // This takes ~1s on initialization because the physical noise is measure using board's timers drift.
    uint32_t seed = physicalRandom32();
    Serial.print(F("Seed: "));
    Serial.println(seed);

    // Use the random physical value as a seed for "regular" random number generation.
    // This will make the random sequence start at an undetermined point everytime the board is reset.
    randomSeed(seed);
}

void loop()
{
    // You can then use `random()` as usual, but the sequence won't ever start from the same point
    // each reset, giving you an actual unpredictable random.
    Serial.println(random(100));
    delay(1000);
}
