#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>
#include <constants.h>
#include <FastLED.h>
#include <Adafruit_PN532.h>


// Each game piece stores a character, NFC UID, a decoder value, and LED positions.
struct GamePiece {
    byte character; // Letters are lowercase ascii values (97-122), numbers are face value (0-20)
    uint8_t uid[MAX_UID_LENGTH];       // NFC tag unique ID (if assigned)
    int positions[WIDTH_PER_PIECE]; // LED indices for this piece on the strip
    int decoder_value;    // Value read from an RFID decoder
};


// We combine both sets of game pieces into one struct for EEPROM storage.
struct GamePieces {
    GamePiece letters[num_letters];
    GamePiece numbers[num_numbers];
};


// Global variables (that are defined "somewhere")
extern GamePieces game_pieces;  // global copy for EEPROM read/write
extern CRGB letter_crgb_leds[num_letter_leds];
extern CRGB number_crgb_leds[num_number_leds];
extern Adafruit_PN532 nfc;


#endif