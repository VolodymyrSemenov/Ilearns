#ifndef CONSTANTS_H
#define CONSTANTS_H

// -------------------------
// Pin Definitions & Constants
// -------------------------

// Recalibrate Button Pin
constexpr int RECALIBRATE_BUTTON_PIN = 2;

// Difficulty Switch / Rocker for selecting between games
constexpr int ENUNCIATION_PIN = 9;

// BUTTONS
// Left side
constexpr int LETTER_ORDERING_BUTTON_PIN = 10; // left red
constexpr int LETTER_WAND_BUTTON_PIN = 11;     // left blue
constexpr int NUMBER_ORDERING_BUTTON_PIN = 12; // left yellow
constexpr int NUMBER_WAND_BUTTON_PIN = 13;     // left green
// Right side
constexpr int HINT_BUTTON_PIN = 18;     // right yellow
constexpr int END_GAME_BUTTON_PIN = 19; // right red
constexpr int REPEAT_BUTTON_PIN = 20;   // right white
constexpr int SKIP_BUTTON_PIN = 21;     // right green

// LEDs
// Left side (+23 offset from their respective button pins)
constexpr int LETTER_ORDERING_LED_PIN = 33;
constexpr int LETTER_WAND_LED_PIN = 34;
constexpr int NUMBER_ORDERING_LED_PIN = 35;
constexpr int NUMBER_WAND_LED_PIN = 36;

// Right side (+23 offset from their respective button pins)
constexpr int HINT_LED_PIN = 41;
constexpr int END_GAME_LED_PIN = 42;
constexpr int REPEAT_LED_PIN = 43;
constexpr int SKIP_LED_PIN = 44;

// WS2811 LED strips
constexpr int LETTER_LED_STRIP_PIN = 38; // WS2811 data pin for letter LEDs
constexpr int NUMBER_LED_STRIP_PIN = 39; // WS2811 data pin for number LEDs



constexpr int WAITING_STATE = 0;
constexpr int LETTER_ORDERING_STATE = 1;
constexpr int LETTER_WAND_STATE = 2;
constexpr int NUMBER_ORDERING_STATE = 3;
constexpr int NUMBER_WAND_STATE = 4;
constexpr int ENUNCIATION_STATE = 5;
constexpr int GAME_OVER_STATE = 6;
constexpr int RECALIBRATING_STATE = 7;
// Decoder Pins
constexpr int decoderPins[] = {26, 27, 28, 29, 30, 31, 32}; // ~EN through A5

// -------------------------
// Constant Values
// -------------------------
constexpr int WIDTH_PER_PIECE = 3;                                                                                     // Number of WS2811 LEDs per game piece
constexpr int BLANK_LEDS_BETWEEN_PIECE = 1;                                                                            // Number of blank LEDs between game pieces
constexpr int BRIGHTNESS = 128;                                                                                        // Brightness of the LEDs (0-255)
constexpr int OFFSET_ARCADE_BUTTON_TO_ARCADE_LED = 23;                                                                 // Offset for arcade buttons
constexpr int MAX_UID_LENGTH = 7;                                                                                      // Maximum UID length for NFC tags
constexpr int OFFSET_BLANK_WS2811_LEDS = 25;                                                                           // offset of number of leds between arduino and the first tile (same for both letters and numbers)
constexpr int OFFSET_GAMESTATE_TO_ARCADE_BUTTON = 9;                                                                   // Distance between arcade button pin and LED strip
constexpr int OFFSET_GAMESTATE_TO_ARCADE_LED = OFFSET_GAMESTATE_TO_ARCADE_BUTTON + OFFSET_ARCADE_BUTTON_TO_ARCADE_LED; // Distance between arcade LED pin and LED strip

constexpr int NUM_LETTERS = 26;
constexpr int NUM_NUMBERS = 21;

constexpr int NUM_LETTER_LEDS = NUM_LETTERS * WIDTH_PER_PIECE + OFFSET_BLANK_WS2811_LEDS;
constexpr int NUM_NUMBER_LEDS = NUM_NUMBERS * WIDTH_PER_PIECE + OFFSET_BLANK_WS2811_LEDS;

constexpr int EEPROM_VALID_BIT_ADDRESS = 0;                              // Value of 1 indicates valid EEPROM data
constexpr int EEPROM_GAME_PIECES_ADDRESS = EEPROM_VALID_BIT_ADDRESS + 1; // Address to store game pieces in EEPROM

// -------------------------
// Game State Variables
// -------------------------
extern volatile int game_state;
extern volatile int utility_button_pressed;

#endif