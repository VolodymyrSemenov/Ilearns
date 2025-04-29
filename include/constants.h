#ifndef CONSTANTS_H
#define CONSTANTS_H


// -------------------------
// Pin Definitions & Constants
// -------------------------

// Difficulty Switch / Rocker for selecting between games
constexpr int annunciation_pin = 9;

// BUTTONS
// Left side
constexpr int letter_ordering_button = 10; // left red
constexpr int letter_wand_button = 11; // left blue
constexpr int number_ordering_button = 12; // left yellow
constexpr int number_wand_button = 13; // left green
// Right side
constexpr int hint_button = 18; // right yellow
constexpr int end_game_button = 19; // right red
constexpr int repeat_button = 20; // right white
constexpr int skip_button = 21; // right green

// Decoder Pins
constexpr int decoderPins[] = {26, 27, 28, 29, 30, 31, 32}; // ~EN through A5

// LEDs
// Left side (+23 offset from their respective button pins)
constexpr int letter_ordering_led = 33; 
constexpr int letter_wand_led = 34;
constexpr int number_ordering_led = 35;
constexpr int number_wand_led = 36;

// WS2811 LED strips
constexpr int letter_led_strip = 38;   // WS2811 data pin for letter LEDs
constexpr int number_led_strip = 39;   // WS2811 data pin for number LEDs

// Right side (+23 offset from their respective button pins)
constexpr int hint_led = 41; 
constexpr int end_game_led = 42;
constexpr int repeat_led = 43;
constexpr int skip_led = 44;

// Recalibrate Button Pin
constexpr int recalibrate_button    = 46;


// -------------------------
// Constant Values
// -------------------------
constexpr int WIDTH_PER_PIECE = 3;   // Number of WS2811 LEDs per game piece
constexpr int BLANK_LEDS_BETWEEN_PIECE = 1; // Number of blank LEDs between game pieces
constexpr int BRIGHTNESS = 128; // Brightness of the LEDs (0-255)
constexpr int ARCADE_LED_OFFSET = 23; // Offset for arcade buttons
constexpr int MAX_UID_LENGTH = 7; // Maximum UID length for NFC tags
constexpr int FRONT_OF_LED_STRIP_OFFSET = 25; // offset of number of leds between arduino and the first tile (same for both letters and numbers)

constexpr int num_letters = 26;
constexpr int num_numbers = 21;

constexpr int num_letter_leds = num_letters * WIDTH_PER_PIECE;
constexpr int num_number_leds = num_numbers * WIDTH_PER_PIECE;

constexpr int eeprom_valid_bit_address = 0; // Value of 1 indicates valid EEPROM data
constexpr int eeprom_game_pieces_address = eeprom_valid_bit_address + 1; // Address to store game pieces in EEPROM


// -------------------------
// Game State Variables
// -------------------------
extern bool game_over;
extern int game_state;


#endif