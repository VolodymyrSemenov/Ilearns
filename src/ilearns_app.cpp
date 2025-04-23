/**************************************************************************
 * Integrated Game Code
**************************************************************************/
#include <constants.h>
#include <structures.h>
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
#include <button_handler.h>


void PiecesToEEPROM();


// -------------------------
// Game State Variables from constants and structures headers
// -------------------------
int game_state = 0; // Game state variable
bool game_over = false;

GamePieces game_pieces;
CRGB letter_crgb_leds[num_letter_leds];
CRGB number_crgb_leds[num_number_leds];
Adafruit_PN532 nfc(53, &SPI);


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


// Print a single game piece
void print_single_game_piece(GamePiece game_piece){
    Serial.print("\nCharacter: "); 
    Serial.println(game_piece.character);

    Serial.print("UID: "); 
    for (uint8_t j = 0; j < MAX_UID_LENGTH; j++) {
        Serial.print(game_piece.uid[j], HEX);
        Serial.print(" ");
    }
    Serial.println("");

    Serial.print("Positions: "); 
    for (int j = 0; j < WIDTH_PER_PIECE; j++) {
        Serial.print(game_piece.positions[j]); 
        Serial.print(" ");
    }
    Serial.println();

    Serial.print("Decoder Value: "); 
    Serial.println(game_piece.decoder_value);
    Serial.println();
}


// Print game pieces to Serial for debugging
void print_game_pieces() {

    Serial.println("\nLetter Game Pieces:");
    for (int i = 0; i < num_letters; i++) {
        print_single_game_piece(game_pieces.letters[i]);
    }

    Serial.println("\nNumber Game Pieces:");
    for (int i = 0; i < num_numbers; i++) {
        print_single_game_piece(game_pieces.numbers[i]);
    }
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
    FastLED.addLeds<WS2811, letter_led_strip, RGB>(letter_crgb_leds, num_letter_leds);
    FastLED.addLeds<WS2811, number_led_strip, RGB>(number_crgb_leds, num_number_leds);
    for (int i = 0; i < num_letter_leds; i++) {
        letter_crgb_leds[i] = CRGB::Black;
    }
    FastLED.setBrightness(128);

    FastLED.show();
    delay(500);
}


// Set the color of a game piece and display it on the LED strip
void set_and_display_gamepiece_color(GamePiece game_piece, CRGB color) {

    Serial.println("set gp color");
    int indice = game_piece.decoder_value;
    Serial.println(indice);
    Serial.println(num_letters);

    if (indice < num_letters){
        Serial.println("set gp color if");
        Serial.println(game_piece.positions[0]);
        for (int i=game_piece.positions[0]; i<game_piece.positions[2]; i++){
            Serial.print("i="); Serial.println(i);
            letter_crgb_leds[i] = color;
        }
    }

    else {
        Serial.println("set gp color else");
        for (int i=game_piece.positions[0]; i<game_piece.positions[2]; i++){
            number_crgb_leds[i] = color;
        }
    }

    FastLED.show();  // Update the LEDs
}


// Given a base index, fill the positions array for a game piece.
void generatePositions(int baseIndex, int positions[]) {
    for (int i = 0; i < WIDTH_PER_PIECE; i++) {
        positions[i] = baseIndex + i;
    }
}


// Populate a single GamePiece struct
GamePiece generate_single_game_piece(GamePiece game_piece, byte character_value, uint8_t uid[], int i) {
    game_piece.character = character_value;
    memcpy(game_piece.uid, uid, MAX_UID_LENGTH);
    game_piece.decoder_value = i;
    generatePositions(i * WIDTH_PER_PIECE, game_piece.positions);

    return game_piece;
}
 

// Generate the gamePiece structures for letters (A-Z) and numbers (0-20)
void generate_game_pieces_structure() {
    int lower_case_ascii_offset = 97; // ASCII value for 'a'. Could also directly use 'a'
    uint8_t uidLength;

    // Letters
    for (int i = 0; i < num_letters; i++) {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs
        Serial.print("Waiting for letter "); Serial.println(char(lower_case_ascii_offset + i));

        // print_single_game_piece(game_pieces.letters[i]);

        // for (int i = 0; i < num_letter_leds; i++) {
        //     letter_crgb_leds[i] = CRGB::Yellow;
        // }
        // FastLED.show();
        int next_starting_position = 0;
        if (i>0){
            next_starting_position = game_pieces.letters[i-1].positions[0] + WIDTH_PER_PIECE;
        }

        for (int i=next_starting_position; i<next_starting_position+2; i++){
            Serial.print("i="); Serial.println(i);
            letter_crgb_leds[i] = CRGB::Yellow;
        }

        set_and_display_gamepiece_color(game_pieces.letters[i], CRGB::Yellow);
        Serial.println("color should bw yellow");

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30)) {
            delay(50);  // avoid overwhelming the RFID reader
        }

        game_pieces.letters[i] = generate_single_game_piece(game_pieces.letters[i], lower_case_ascii_offset + i, uid, i);
        print_single_game_piece(game_pieces.letters[i]);
        set_and_display_gamepiece_color(game_pieces.letters[i], CRGB::Green);
        // for (int i = 0; i < num_letter_leds; i++) {
        //     letter_crgb_leds[i] = CRGB::Green;
        // }
        // FastLED.show();

        Serial.println("color should bw green");
        delay(1000);
    }

    // Numbers
    for (int i = 0; i < num_numbers; i++) {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs

        set_and_display_gamepiece_color(game_pieces.numbers[i], CRGB::Yellow);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30)) {
            delay(50);  // avoid overwhelming the RFID reader
        }

        generate_single_game_piece(game_pieces.numbers[i], i, uid, i);
        set_and_display_gamepiece_color(game_pieces.numbers[i], CRGB::Green);
        delay(1000);
    }
}


// Write entire GamePieces struct to EEPROM (at game_pieces_address)
void PiecesToEEPROM() {
    EEPROM.put(eeprom_valid_bit_address, 1);
    EEPROM.put(eeprom_game_pieces_address, game_pieces);
}


// Read the GamePieces struct from EEPROM
GamePieces PiecesFromEEPROM() {
    GamePieces gp;
    EEPROM.get(eeprom_game_pieces_address, gp);
    return gp;
}


// Recalibrate game pieces and write to EEPROM
void recalibrate_game_pieces() {
    generate_game_pieces_structure();
    PiecesToEEPROM();
}


// Populate GamePieces struct 
void populate_game_pieces_structure(){
    int valid_bit = EEPROM.read(eeprom_valid_bit_address);
    if (valid_bit == 1){
        game_pieces = PiecesFromEEPROM();
    }
    else {
        recalibrate_game_pieces();
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
    
    nfc.begin();
    nfc.setPassiveActivationRetries(0xFF);
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN532 board");
        while (1); // Halt
    }
    Serial.println("NFC reader initialized.");

    
    Serial.println("EEPROM Initializing");
    recalibrate_game_pieces();
    print_game_pieces();

    Serial.println("Press a button to start a game.");
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
//         recalibrate_game_pieces();
//     }

//     delay(100);
// }
