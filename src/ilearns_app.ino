/**************************************************************************
 * Integrated Game Code
**************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <FastLED.h>
#include <EEPROM.h>

// Include Game Logic
#include <../games/letter_ordering.ino>
#include <../games/number_ordering.ino>
#include <../games/find_letters_spoken.ino>
#include <../games/find_letters_annunciation.ino>
#include <../games/find_numbers.ino>

// -------------------------
// Pin Definitions & Constants
// -------------------------
#define LED_PIN_LETTERS 6   // WS2811 data pin for letter LEDs
#define LED_PIN_NUMBERS 7   // WS2811 data pin for number LEDs
#define WIDTH_PER_PIECE 3   // Number of WS2811 LEDs per game piece

// Button Pins
const int letterOrderingBtn = 2;
const int numberOrderingBtn   = 3;
const int findLettersBtn      = 4;
const int findNumbersBtn      = 5;

// LEDs for button indicators
const int letterOrderingLED   = 6;  // Same as LED strip pin (conflict resolved by design)
const int numberOrderingLED   = 7;
const int findLettersLED      = 8;
const int findNumbersLED      = 9;

// Rest of functionality buttons
const int endGameBtnPin       = 11;
const int skipBtn             = 12;
const int repeatBtn           = 13;
const int extraBtn            = 14;

// LEDs for functionality buttons
const int endGameLEDPin       = 17; 
const int skipLEDPin          = 18;
const int repeatLEDPin        = 19;
const int extraLEDPin         = 20;

// Recalibrate Button Pin
const int recalibrateBtnPin    = 21;

// Game constants
const int num_letters = 26;
const int num_numbers = 21;
const int num_letter_leds = num_letters * WIDTH_PER_PIECE;
const int num_number_leds = num_numbers * WIDTH_PER_PIECE;

// -------------------------
// Data Structures
// -------------------------

// Each game piece stores a character, NFC UID, a decoder value, and LED positions.
struct gamePiece {
    char character[3];    // e.g., "A", "B", ... or "20", "1", etc.
    uint8_t uid[7];       // NFC tag unique ID (if assigned)
    int decoder_value;    // Value read from an RFID decoder
    int positions[WIDTH_PER_PIECE]; // LED indices for this piece on the strip
};

// Global arrays for game pieces (letters and numbers)
gamePiece letters[num_letters];
gamePiece numbers[num_numbers];

// LED arrays for FastLED (each strip holds all LEDs for that group)
CRGB letter_crgb_leds[num_letter_leds];
CRGB number_crgb_leds[num_number_leds];

// -------------------------
// NFC (PN532) Setup
// -------------------------
// Using SPI with a custom set of decoder pins for this example.
uint8_t decoderPins[] = {8, 7, 6, 5, 4, 3, 2}; // Will change later depending on design
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

// Write entire GamePieces struct to EEPROM (starting at address 0)
void PiecesToEEPROM() {
    EEPROM.put(0, gamePiecesGlobal);
}

// Read the GamePieces struct from EEPROM
GamePieces PiecesFromEEPROM() {
    GamePieces gp;
    EEPROM.get(0, gp);
    return gp;
}

// -------------------------
// LED Utility Functions
// -------------------------

// Initialize both WS2811 LED strips (one for letters, one for numbers)
void initializeLEDs() {
    FastLED.addLeds<WS2811, LED_PIN_LETTERS, RGB>(letter_crgb_leds, num_letter_leds);
    FastLED.addLeds<WS2811, LED_PIN_NUMBERS, RGB>(number_crgb_leds, num_number_leds);
    FastLED.show();
    FastLED.setBrightness(128);
}

// -------------------------
// Game Piece Helper Functions
// -------------------------

// Given a base index, fill the positions array for a game piece.
void generatePositions(int baseIndex, int positions[]) {
    for (int i = 0; i < WIDTH_PER_PIECE; i++) {
        positions[i] = baseIndex + i;
    }
}

// Generate the gamePiece structures for letters (A-Z) and numbers (0-20)
void generateGamePieceStructures() {
    // Letters: assign characters, clear UIDs, set decoder value, and assign LED positions.
    for (int i = 0; i < num_letters; i++) {
        // Use uppercase letters – adjust if you prefer lowercase
        snprintf(letters[i].character, sizeof(letters[i].character), "%c", 'A' + i);
        memset(letters[i].uid, 0, sizeof(letters[i].uid));
        letters[i].decoder_value = i;
        generatePositions(i * WIDTH_PER_PIECE, letters[i].positions);
    }
    // Numbers: assign string versions of numbers 0 to 20
    for (int i = 0; i < num_numbers; i++) {
        snprintf(numbers[i].character, sizeof(numbers[i].character), "%d", i);
        memset(numbers[i].uid, 0, sizeof(numbers[i].uid));
        numbers[i].decoder_value = i;
        generatePositions(i * WIDTH_PER_PIECE, numbers[i].positions);
    }
}

// Print game pieces to Serial for debugging
void printGamePieces() {
    Serial.println("Letter Game Pieces:");
    for (int i = 0; i < num_letters; i++) {
        Serial.print("Character: "); Serial.println(letters[i].character);
        Serial.print("Positions: "); 
        for (int j = 0; j < WIDTH_PER_PIECE; j++) {
        Serial.print(letters[i].positions[j]); 
        Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println("Number Game Pieces:");
    for (int i = 0; i < num_numbers; i++) {
        Serial.print("Character: "); Serial.println(numbers[i].character);
        Serial.print("Positions: "); 
        for (int j = 0; j < WIDTH_PER_PIECE; j++) {
        Serial.print(numbers[i].positions[j]); 
        Serial.print(" ");
        }
        Serial.println();
    }
}

// -------------------------
// Button Checking Function
// -------------------------

// Checks the four game buttons and returns true if one is pressed.
// Sets button_number to the pressed button’s defined constant.
bool check_button_pressed(int &button_number) {
    if (digitalRead(letterOrderingBtn) == LOW) {
        button_number = letterOrderingBtn;
        return true;
    } else if (digitalRead(numberOrderingBtn) == LOW) {
        button_number = numberOrderingBtn;
        return true;
    } else if (digitalRead(findLettersBtn) == LOW) {
        button_number = findLettersBtn;
        return true;
    } else if (digitalRead(findNumbersBtn) == LOW) {
        button_number = findNumbersBtn;
        return true;
    }
    return false;
}

// -------------------------
// NFC & EEPROM Recalibration
// -------------------------

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
        }
    }
    PiecesToEEPROM();
}

// -------------------------
// Arduino Setup and Loop
// -------------------------

void setup() {
    Serial.begin(9600);
    // Initialize button pins with internal pullups
    pinMode(letterOrderingBtn, INPUT_PULLUP);
    pinMode(numberOrderingBtn, INPUT_PULLUP);
    pinMode(findLettersBtn, INPUT_PULLUP);
    pinMode(findNumbersBtn, INPUT_PULLUP);
    pinMode(endGameBtnPin, INPUT_PULLUP); // Initialize End Game Button
    
    // Rest of functionality buttons
    pinMode(skipBtn, INPUT_PULLUP);
    pinMode(repeatBtn, INPUT_PULLUP);
    pinMode(extraBtn, INPUT_PULLUP);
    pinMode(endGameLEDPin, OUTPUT);
    pinMode(skipLEDPin, OUTPUT);
    pinMode(repeatLEDPin, OUTPUT);
    pinMode(extraLEDPin, OUTPUT);
    digitalWrite(skipLEDPin, HIGH);
    digitalWrite(repeatLEDPin, HIGH);
    digitalWrite(extraLEDPin, HIGH);
    digitalWrite(endGameLEDPin, HIGH);
    
    // Initialize WS2811 LED strips for game pieces
    delay(500);
    Serial.println("Initializing Letter LEDs...");
    delay(500);
    Serial.println("Initializing Number LEDs...");
    initializeLEDs();
    delay(500);
    
    // Initialize NFC reader
    nfc.begin();
    nfc.setPassiveActivationRetries(0xFF);
    Serial.println("NFC reader initialized.");
    
    // Attempt to load game pieces from EEPROM.
    // If EEPROM is empty (first letter is empty), generate the structures.
    GamePieces stored = PiecesFromEEPROM();
    if (stored.letters[0].character[0] == '\\0') {
    Serial.println("EEPROM empty, generating game pieces...");
    generateGamePieceStructures();
    // Copy into global structure for EEPROM storage
    for (int i = 0; i < num_letters; i++) {
        gamePiecesGlobal.letters[i] = letters[i];
    }
    for (int i = 0; i < num_numbers; i++) {
        gamePiecesGlobal.numbers[i] = numbers[i];
    }
    PiecesToEEPROM();
    } else {
    Serial.println("Loaded game pieces from EEPROM.");
    // Copy stored values into our working arrays
    for (int i = 0; i < num_letters; i++) {
        letters[i] = stored.letters[i];
    }
    for (int i = 0; i < num_numbers; i++) {
        numbers[i] = stored.numbers[i];
    }
    }
    
    printGamePieces();
    Serial.println("Press a button to start a game.");
}

void loop() {
// Game Buttons
    int button_number;
    if (check_button_pressed(button_number)) {
        switch (button_number) {
        case letterOrderingBtn:
            begin_game_letter_ordering();
            break;
        case numberOrderingBtn:
            begin_game_number_ordering();
            break;
        case findLettersBtn:
            // Check difficulty and choose one of the find letters games
            // begin_game_find_letters_spoken();
            // begin_game_find_letters_annunciation();
            break;
        case findNumbersBtn:
            begin_game_find_numbers();
            break;
        default:
            Serial.println("Unknown button pressed.");
        }
        delay(1000); // Basic debounce/delay between games
    }

    // Check if recalibrate is being held down
    if(digitalRead(recalibrateBtnPin) == LOW){
        recalibrateGamePieces();
    }

    delay(100);
}
