/**
 * Example for generating a random ID by using the `PhysicalRandom` utility, and store that value
 * so it survive resets.
 *
 * Open the Serial Monitor at 9600 baud. Single-character commands:
 *   p  Print the stored ID.
 *   r  Re-roll: clear the record and generate a new ID.
 *   c  Clear the EEPROM record, so the next boot behaves like a factory-fresh chip.
 */

#include <EEPROM.h>

#include "PhysicalRandom.h"
#include "Utilities.h"

/** Marks that a valid ID has been written. A freshly erased EEPROM never holds this value. */
#define EEPROM_FLAG_VALUE 0xA5
#define EEPROM_FLAG_ADDRESS 0
#define EEPROM_ID_ADDRESS (EEPROM_FLAG_ADDRESS + 1)

/** Stores the ID of this device, loaded or generated on setup. */
static uint64_t id = 0;

void setup()
{
    Serial.begin(9600);

    // Try to load an existing ID, or generate a new one for this board. This will feed the `id`
    // variable so you can print that value 
    loadOrGenerateID();
    printID();
    Serial.println(F("Commands: p = print ID, r = re-roll ID, c = clear ID"));
}

void loop()
{
    // Cancel the loop if Serial is not available
    if (!Serial.available())
        return;

    // Read the Serial input if any
    switch (Serial.read())
    {
        case 'p':
            printID();
            break;

        case 'r':
            clearID();
            loadOrGenerateID();
            Serial.println(F("ID re-rolled:"));
            printID();
            break;

        case 'c':
            clearID();
            Serial.println(F("ID cleared. A new ID will be generated on the next reset."));
            break;

        default:
            break;
    }
}

/** Loads the stored ID, or generates and stores a new one if none exists yet. */
static void loadOrGenerateID()
{
    // We only check for the flag. If it's defined, then we assume the ID exists.
    if (EEPROM.read(EEPROM_FLAG_ADDRESS) == EEPROM_FLAG_VALUE)
    {
        // Read the existing ID and stop there.
        EEPROM.get(EEPROM_ID_ADDRESS, id);
        return;
    }

    // Generate a new 64-bit ID.
    id = physicalRandom64();

    // Store the generated ID in persistent memory, so it will survive board resets.
    EEPROM.put(EEPROM_ID_ADDRESS, id);
    EEPROM.update(EEPROM_FLAG_ADDRESS, EEPROM_FLAG_VALUE);
}

/** Clear the ID flag, so a new ID will be generated on next reset. */
static void clearID()
{
    EEPROM.update(EEPROM_FLAG_ADDRESS, 0xFF);
}

/** Converts the ID (a 64-bit value) into readable hexadecimal text and print it. */
static void printID()
{
    char text[17]; // 16 hex digits + null terminator
    bytes64ToHex(id, text);
    Serial.print(F("ID: "));
    Serial.println(text);
}
