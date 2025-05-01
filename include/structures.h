#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <FastLED.h>
#include <Adafruit_PN532.h>
#include <constants.h>

// Each game piece stores a character, NFC UID, a decoder value, and LED positions.
struct GamePiece
{
    byte character;                 // Letters are lowercase ascii values (97-122), numbers are face value (0-20)
    uint8_t uid[MAX_UID_LENGTH];    // NFC tag unique ID (if assigned)
    int positions[WIDTH_PER_PIECE]; // LED indices for this piece on the strip
    int decoder_value;              // Value read from an RFID decoder
};

// We combine both sets of game pieces into one struct for EEPROM storage.
struct GamePieces
{
    GamePiece letters[NUM_LETTERS];
    GamePiece numbers[NUM_NUMBERS];
};

// Global variables (that are defined "somewhere")
extern GamePieces game_pieces; // global copy for EEPROM read/write
extern CRGB letter_crgb_leds[NUM_LETTER_LEDS];
extern CRGB number_crgb_leds[NUM_NUMBER_LEDS];
extern Adafruit_PN532 nfc;

#endif