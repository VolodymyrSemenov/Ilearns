/**************************************************************************
 * Integrated Game Code
**************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <FastLED.h>
#include <EEPROM.h>
#include <EnableInterrupt.h>

// Include Game Logic
// #include <../games/letter_ordering.ino>
// #include <../games/number_ordering.ino>
// #include <../games/find_letters_spoken.ino>
// #include <../games/find_letters_annunciation.ino>
// #include <../games/find_numbers.ino>
#include <button_handler.cpp>


// -------------------------
// Pin Definitions & Constants
// -------------------------

// Difficulty Switch / Rocker for selecting between games
const int annunciation_pin = 9;

// BUTTONS
// Left side
const int letter_ordering_button   = 10; // left red
const int letter_wand_button      = 11; // left blue
const int number_ordering_button   = 12; // left yellow
const int number_wand_button      = 13; // left green
// Right side
const int hint_button             = 18; // right yellow
const int end_game_button          = 19; // right red
const int repeat_button           = 20; // right white
const int skip_button             = 21; // right green

// Decoder Pins
uint8_t decoderPins[] = {26, 27, 28, 29, 30, 31, 32}; // ~EN through A5

// LEDs
// Left side (+23 offset from their respective button pins)
const int letter_ordering_led   = 33; 
const int letter_wand_led      = 34;
const int number_ordering_led   = 35;
const int number_wand_led      = 36;

// WS2811 LED strips
#define LED_PIN_LETTERS 38   // WS2811 data pin for letter LEDs
#define LED_PIN_NUMBERS 39   // WS2811 data pin for number LEDs

// Right side (+23 offset from their respective button pins)
const int end_game_led       = 41; 
const int skip_led          = 42;
const int repeat_led        = 43;
const int extra_led         = 44;

// Recalibrate Button Pin
const int recalibrate_button    = 46;


// -------------------------
// Game Constant Values
// -------------------------
#define WIDTH_PER_PIECE 3   // Number of WS2811 LEDs per game piece
#define BRIGHTNESS 128 // Brightness of the LEDs (0-255)
#define ARCADE_LED_OFFSET 23 // Offset for arcade buttons

const int num_letters = 26;
const int num_numbers = 21;
const int num_letter_leds = num_letters * WIDTH_PER_PIECE;
const int num_number_leds = num_numbers * WIDTH_PER_PIECE;

// void PiecesToEEPROM();
// void generatePositions();


// -------------------------
// Game State Variables
// -------------------------
int game_state = 0; // Game state variable
bool game_over = false;


// -------------------------
// Data Structures
// -------------------------

// Each game piece stores a character, NFC UID, a decoder value, and LED positions.
struct gamePiece {
    // char character[3];    // e.g., "A", "B", ... or "20", "1", etc.
    byte character;
    uint8_t uid[7];       // NFC tag unique ID (if assigned)
    int decoder_value;    // Value read from an RFID decoder
    int positions[WIDTH_PER_PIECE]; // LED indices for this piece on the strip
};

// Global arrays for game pieces (letters and numbers)
// gamePiece letters[num_letters];
// gamePiece numbers[num_numbers];

// LED arrays for FastLED (each strip holds all LEDs for that group)
CRGB letter_crgb_leds[num_letter_leds];
CRGB number_crgb_leds[num_number_leds];


// -------------------------
// NFC (PN532) Setup
// -------------------------

// Using SPI with a custom set of decoder pins for this example.
Adafruit_PN532 nfc(decoderPins, &SPI, 10);


// -------------------------
// EEPROM Handling
// -------------------------

// We combine both sets of game pieces into one struct for EEPROM storage.
struct GamePieces {
    gamePiece letters[num_letters];
    gamePiece numbers[num_numbers];
};

GamePieces gamePiecesGlobal;  // global copy for EEPROM read/write

// This stub function attempts to read a card for each game piece slot (total 47).
// It then updates the gamePiecesGlobal with the new UID values.
void recalibrate_game_pieces() {
    uint8_t uid[7];
    uint8_t uidLength;
    // There are 47 pieces total: 26 letters + 21 numbers.
    for (int i = 0; i < 47; i++) {
        bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
        if (success) {
            if (i < num_letters) {
                memcpy(gamePiecesGlobal.letters[i].uid, uid, uidLength);
            } else {
                memcpy(gamePiecesGlobal.numbers[i - num_letters].uid, uid, uidLength);
            }
        }
    }
    PiecesToEEPROM();
}

// Subroutine to start a rainbow LED dance when game_over is true.
// New subroutine to start a rainbow LED dance for 5 seconds when game_over is true.
void startLEDRainbowDance() {
    unsigned long startTime = millis(); // Record the start time

    // Run a rainbow pattern for 5 seconds.
    while (millis() - startTime < 5000) {
        static uint8_t hue = 0;
        fill_rainbow(letter_crgb_leds, num_letter_leds, hue, 8);
        fill_rainbow(number_crgb_leds, num_number_leds, hue, 8);
        FastLED.show();
        hue += 5; // Gradually shift colors
        delay(50);
    }

    // After 5 seconds, clear the LEDs
    FastLED.clear();
    FastLED.show();
}



// Gives buttons pull up resistors and sets leds as outputs
void set_pin_modes(){

    // Initialize button pins with internal pullups
    pinMode(letter_ordering_button, INPUT_PULLUP);
    pinMode(number_ordering_button, INPUT_PULLUP);
    pinMode(annunciation_pin, INPUT_PULLUP);
    pinMode(letter_wand_button, INPUT_PULLUP);
    pinMode(number_wand_button, INPUT_PULLUP);
    
    pinMode(end_game_button, INPUT_PULLUP); 
    pinMode(skip_button, INPUT_PULLUP);
    pinMode(repeat_button, INPUT_PULLUP);
    pinMode(hint_button, INPUT_PULLUP);

    // Set led pins as outputs
    pinMode(letter_ordering_led, OUTPUT);
    pinMode(number_ordering_led, OUTPUT);
    pinMode(letter_wand_led, OUTPUT);
    pinMode(number_wand_led, OUTPUT);

    pinMode(end_game_led, OUTPUT);
    pinMode(skip_led, OUTPUT);
    pinMode(repeat_led, OUTPUT);
    pinMode(extra_led, OUTPUT);
}


// Enables interrupts on buttons
void enable_interrupts(){
    enableInterrupt(letter_ordering_button, letter_ordering_button_handler, FALLING);
    enableInterrupt(letter_wand_button, letter_wand_button_handler, FALLING);
    enableInterrupt(number_ordering_button, number_ordering_button_handler, FALLING);
    enableInterrupt(number_wand_button, number_wand_button_handler, FALLING);

    enableInterrupt(hint_button, hint_button_handler, FALLING);
    enableInterrupt(end_game_button, end_game_button_handler, FALLING);
    enableInterrupt(repeat_button, repeat_button_handler, FALLING);
    enableInterrupt(skip_button, skip_button_handler, FALLING);

    enableInterrupt(recalibrate_button, recalibrate_button_handler, FALLING);
}


// Initialize both WS2811 LED strips (one for letters, one for numbers)
void initialize_led_strips() {
    FastLED.addLeds<WS2811, LED_PIN_LETTERS, RGB>(letter_crgb_leds, num_letter_leds);
    FastLED.addLeds<WS2811, LED_PIN_NUMBERS, RGB>(number_crgb_leds, num_number_leds);
    FastLED.show();
    FastLED.setBrightness(128);
}


// Read the GamePieces struct from EEPROM
GamePieces PiecesFromEEPROM() {
    GamePieces gp;
    EEPROM.get(0, gp);
    return gp;
}


// Given a base index, fill the positions array for a game piece.
void generatePositions(int baseIndex, int positions[]) {
    for (int i = 0; i < WIDTH_PER_PIECE; i++) {
        positions[i] = baseIndex + i;
    }
}
 

// Generate the gamePiece structures for letters (A-Z) and numbers (0-20)
void generateGamePieceStructures() {
    int lower_case_ascii_offset = 97; // ASCII value for 'a'
    // Letters
    for (int i = 0; i < num_letters; i++) {
        // Use uppercase letters – adjust if you prefer lowercase
        // snprintf(letters[i].character, sizeof(letters[i].character), "%c", 'a' + i);
        gamePiecesGlobal.letters[i].character = lower_case_ascii_offset + i;
        memset(gamePiecesGlobal.letters[i].uid, 0, sizeof(gamePiecesGlobal.letters[i].uid));
        gamePiecesGlobal.letters[i].decoder_value = i;
        generatePositions(i * WIDTH_PER_PIECE, gamePiecesGlobal.letters[i].positions);
    }
    // Numbers
    for (int i = 0; i < num_numbers; i++) {
        // snprintf(numbers[i].character, sizeof(numbers[i].character), "%d", i);
        gamePiecesGlobal.numbers[i].character = i;
        memset(gamePiecesGlobal.numbers[i].uid, 0, sizeof(gamePiecesGlobal.numbers[i].uid));
        gamePiecesGlobal.numbers[i].decoder_value = i;
        generatePositions(i * WIDTH_PER_PIECE, gamePiecesGlobal.numbers[i].positions);
    }
}


// Write entire GamePieces struct to EEPROM (starting at address 0)
void PiecesToEEPROM() {
    EEPROM.put(0, gamePiecesGlobal);
}


// Print game pieces to Serial for debugging
void printGamePieces() {
    Serial.println("Letter Game Pieces:");
    for (int i = 0; i < num_letters; i++) {
        Serial.print("Character: "); Serial.println(gamePiecesGlobal.letters[i].character);
        Serial.print("Positions: "); 
        for (int j = 0; j < WIDTH_PER_PIECE; j++) {
            Serial.print(gamePiecesGlobal.letters[i].positions[j]); 
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println("Number Game Pieces:");
    for (int i = 0; i < num_numbers; i++) {
        Serial.print("Character: "); Serial.println(gamePiecesGlobal.numbers[i].character);
        Serial.print("Positions: "); 
        for (int j = 0; j < WIDTH_PER_PIECE; j++) {
            Serial.print(gamePiecesGlobal.numbers[i].positions[j]); 
            Serial.print(" ");
        }
        Serial.println();
    }
}


// -------------------------
// Arduino Setup
// -------------------------

void setup() {
    Serial.begin(9600);

    Serial.println("Setting pin modes");
    set_pin_modes();

    Serial.println("Enable button interrupts");
    enable_interrupts();

    Serial.println("Initializing LED strips");
    initialize_led_strips();
    
    // Initialize NFC reader
    nfc.begin();
    nfc.setPassiveActivationRetries(0xFF);
    Serial.println("NFC reader initialized.");

    Serial.println("EEPROM Initializing");
    GamePieces stored = PiecesFromEEPROM();
    if (stored.letters[0].character[0] != 0) {
        Serial.println("EEPROM empty, generating game pieces...");
        generateGamePieceStructures();
        // Copy into global structure for EEPROM storage
        // for (int i = 0; i < num_letters; i++) {
        //     gamePiecesGlobal.letters[i] = letters[i];
        // }
        // for (int i = 0; i < num_numbers; i++) {
        //     gamePiecesGlobal.numbers[i] = numbers[i];
        // }
        PiecesToEEPROM();
    } else {
        Serial.println("Loaded game pieces from EEPROM.");
        // Copy stored values into our working arrays
        for (int i = 0; i < num_letters; i++) {
            gamePiecesGlobal.letters[i] = stored.letters[i];
        }
        for (int i = 0; i < num_numbers; i++) {
            gamePiecesGlobal.numbers[i] = stored.numbers[i];
        }
    }
    printGamePieces();

    Serial.println("Press a button to start a game.");
}

// This stub function attempts to read a card for each game piece slot (total 47).
// It then updates the gamePiecesGlobal with the new UID values.
void recalibrateGamePieces() {
    uint8_t uid[7];
    uint8_t uidLength;
    // There are 47 pieces total: 26 letters + 21 numbers.
    for (int i = 0; i < 47; i++) {
        bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
        if (success) {
            if (i < num_letters) {
                memcpy(gamePiecesGlobal.letters[i].uid, uid, uidLength);
            } else {
                memcpy(gamePiecesGlobal.numbers[i - num_letters].uid, uid, uidLength);
            }
            Serial.print("scanned i="); Serial.print(i);
            delay(250);
        }
    }
    PiecesToEEPROM();
}


// -------------------------
// Arduino Loop
// -------------------------

void loop() {
    // Game Buttons
    // int button_number;
    // if (check_button_pressed(button_number)) {
    //     switch (button_number) {
    //         case letter_ordering_button:
    //             begin_game_letter_ordering();
    //             break;
    //         case number_ordering_button:
    //             begin_game_number_ordering();
    //             break;
    //         case letter_wand_button:
    //             // Check the annunciation rocker switch to choose between games.
    //             // If annunciation_pin reads LOW, select the annunciation game.
    //             if (digitalRead(annunciation_pin) == LOW) {
    //                 begin_game_find_letters_annunciation();
    //             } else {
    //                 begin_game_find_letters_spoken();
    //             }
    //             break;
    //         case number_wand_button:
    //             begin_game_find_numbers();
    //             break;
    //         default:
    //             Serial.println("Unknown button pressed.");
    //     }
    //     delay(1000); // Basic debounce/delay between games
        
        // After a game ends, if game_over is true, start the LED rainbow dance.
        if (game_over) {
            startLEDRainbowDance();
            game_over = false; // Reset the flag
        }
    }

    // Check if recalibrate is being held down
//     if(digitalRead(recalibrate_button_led) == LOW){
//         recalibrateGamePieces();
//     }

//     delay(100);
// }
