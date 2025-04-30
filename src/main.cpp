/**************************************************************************
 * Integrated Game Code
 **************************************************************************/
#include <structures.h>
#include <EEPROM.h>
#include <EnableInterrupt.h>
#include <button_handler.h>
#include <games.h>
#include <printing.h>
#include <illumination.h>

// -------------------------
// Game State Variables from constants and structures headers
// -------------------------
volatile int game_state = 0; // Game state variable

GamePieces game_pieces;
CRGB letter_crgb_leds[num_letter_leds];
CRGB number_crgb_leds[num_number_leds];
Adafruit_PN532 nfc(53, &SPI);

// Gives buttons pull up resistors and sets leds as outputs
void set_pin_modes()
{

    // Initialize button pins with internal pullups
    pinMode(letter_ordering_button, INPUT_PULLUP);
    pinMode(number_ordering_button, INPUT_PULLUP);
    pinMode(letter_wand_button, INPUT_PULLUP);
    pinMode(number_wand_button, INPUT_PULLUP);

    pinMode(end_game_button, INPUT_PULLUP);
    pinMode(skip_button, INPUT_PULLUP);
    pinMode(repeat_button, INPUT_PULLUP);
    pinMode(hint_button, INPUT_PULLUP);

    pinMode(annunciation_pin, INPUT_PULLUP);
    pinMode(recalibrate_button, INPUT_PULLUP);

    // Set led pins as outputs
    pinMode(letter_ordering_led, OUTPUT);
    pinMode(number_ordering_led, OUTPUT);
    pinMode(letter_wand_led, OUTPUT);
    pinMode(number_wand_led, OUTPUT);

    pinMode(end_game_led, OUTPUT);
    pinMode(skip_led, OUTPUT);
    pinMode(repeat_led, OUTPUT);
    pinMode(skip_led, OUTPUT);
}

// Enables interrupts on buttons
void enable_interrupts()
{
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
void initialize_led_strips()
{
    FastLED.addLeds<WS2811, letter_led_strip, RGB>(letter_crgb_leds, num_letter_leds);
    FastLED.addLeds<WS2811, number_led_strip, RGB>(number_crgb_leds, num_number_leds);
    for (int i = 0; i < num_letter_leds; i++)
    {
        letter_crgb_leds[i] = CRGB::Black;
    }
    FastLED.setBrightness(128);

    FastLED.show();
}

// Given a base index, fill the positions array for a game piece.
void generatePositions(int baseIndex, int positions[])
{
    for (int i = 0; i < WIDTH_PER_PIECE; i++)
    {
        positions[i] = baseIndex + i;
    }
}

// Populate a single GamePiece struct
GamePiece generate_single_game_piece(GamePiece game_piece, byte character_value, uint8_t uid[], int decoder_value, int i)
{
    game_piece.character = character_value;
    memcpy(game_piece.uid, uid, MAX_UID_LENGTH);
    game_piece.decoder_value = decoder_value;
    generatePositions(FRONT_OF_LED_STRIP_OFFSET + i * WIDTH_PER_PIECE, game_piece.positions);

    return game_piece;
}

// Generate the gamePiece structures for letters (A-Z) and numbers (0-20)
void generate_game_pieces_structure()
{
    int lower_case_ascii_offset = 97; // ASCII value for 'a'. Could also directly use 'a'
    uint8_t uidLength;

    // Letters
    for (int i = 0; i < num_letters; i++)
    {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs
        Serial.println(i);
        Serial.println(FRONT_OF_LED_STRIP_OFFSET);
        illuminate_next_letter_tile_location(i, CRGB::Yellow);

        Serial.println("size of letter_crgb_leds");
        Serial.println(sizeof(letter_crgb_leds) / 3);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        game_pieces.letters[i] = generate_single_game_piece(game_pieces.letters[i], lower_case_ascii_offset + i, uid, i, i);
        illuminate_single_game_piece(game_pieces.letters[i], CRGB::Green);

        print_single_game_piece(game_pieces.letters[i]);
        delay(500);
    }

    // Numbers
    for (int i = 0; i < num_numbers; i++)
    {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs

        illuminate_next_number_tile_location(i, CRGB::Yellow);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        game_pieces.numbers[i] = generate_single_game_piece(game_pieces.numbers[i], i, uid, i + num_letters, i);
        illuminate_single_game_piece(game_pieces.numbers[i], CRGB::Green);

        print_single_game_piece(game_pieces.numbers[i]);
        delay(500);
    }
}

// Write entire GamePieces struct to EEPROM (at game_pieces_address)
void PiecesToEEPROM()
{
    EEPROM.put(eeprom_valid_bit_address, 1);
    EEPROM.put(eeprom_game_pieces_address, game_pieces);
}

// Read the GamePieces struct from EEPROM
GamePieces PiecesFromEEPROM()
{
    GamePieces gp;
    EEPROM.get(eeprom_game_pieces_address, gp);
    return gp;
}

// Recalibrate game pieces and write to EEPROM
void recalibrate_game_pieces()
{
    generate_game_pieces_structure();
    PiecesToEEPROM();
}

// Populate GamePieces struct
void populate_game_pieces_structure()
{
    int valid_bit = EEPROM.read(eeprom_valid_bit_address);
    if (valid_bit == 1)
    {
        game_pieces = PiecesFromEEPROM();
    }
    else
    {
        recalibrate_game_pieces();
    }
}

void setup()
{
    Serial.begin(115200);

    Serial.println("Setting pin modes");
    set_pin_modes();

    Serial.println("Enable button interrupts");
    enable_interrupts();

    Serial.println("Initializing LED strips");
    initialize_led_strips();

    Serial.println("Initializing NFC reader");
    nfc.begin();
    nfc.setPassiveActivationRetries(0xFF);
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
        Serial.println("Didn't find PN532 board");
        while (true)
        {
        }; // Halt
    }
    Serial.println("NFC reader initialized.");

    Serial.println("EEPROM Initializing");
    populate_game_pieces_structure();
    print_game_pieces();

    fill_letters_solid(CRGB::Blue);
    fill_numbers_solid(CRGB::Red);

    Serial.println("Press a button to start a game.");
}

void loop()
{
    static int previous_state = game_state;
    if (game_state != previous_state) {
        Serial.print("Switching State: ");
        Serial.println(game_state);
        previous_state = game_state;
    }
    if (game_state == GAME_OVER_STATE)
    {
        startLEDRainbowDance();
        game_state = WAITING_STATE;
    }
    if (game_state == RECALIBRATING_STATE)
    {
        Serial.println("Starting recalibration");
        recalibrate_game_pieces();
        game_state = WAITING_STATE;
    }
    if (game_state == LETTER_WAND_STATE)
    {
        Serial.println("Starting letter wand game");
        begin_wand_game();
        game_state = GAME_OVER_STATE;
    }
    if (game_state == NUMBER_WAND_STATE)
    {
        Serial.println("Starting number wand game");
        begin_wand_game();
        game_state = GAME_OVER_STATE;
    }
}
