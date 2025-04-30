/**************************************************************************
 * Integrated Game Code
 **************************************************************************/
#include <EnableInterrupt.h>

#include <structures.h>
#include <button_handler.h>
#include <games.h>
#include <printing.h>
#include <illumination.h>
#include <recalibrate.h>

// -------------------------
// Game State Variables from constants and structures headers
// -------------------------
volatile int game_state = 0; // Game state variable

GamePieces game_pieces;
CRGB letter_crgb_leds[NUM_LETTER_LEDS];
CRGB number_crgb_leds[NUM_NUMBER_LEDS];
Adafruit_PN532 nfc(53, &SPI);

// Gives buttons pull up resistors and sets leds as outputs
void set_pin_modes()
{

    // Initialize button pins with internal pullups
    pinMode(LETTER_ORDERING_BUTTON_PIN, INPUT_PULLUP);
    pinMode(NUMBER_ORDERING_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LETTER_WAND_BUTTON_PIN, INPUT_PULLUP);
    pinMode(NUMBER_WAND_BUTTON_PIN, INPUT_PULLUP);

    pinMode(END_GAME_BUTTON_PIN, INPUT_PULLUP);
    pinMode(SKIP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(REPEAT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(HINT_BUTTON_PIN, INPUT_PULLUP);

    pinMode(ENUNCIATION_PIN, INPUT_PULLUP);
    pinMode(RECALIBRATE_BUTTON, INPUT_PULLUP);

    // Set led pins as outputs
    pinMode(letter_ordering_led, OUTPUT);
    pinMode(number_ordering_led, OUTPUT);
    pinMode(letter_wand_led, OUTPUT);
    pinMode(number_wand_led, OUTPUT);

    pinMode(END_GAME_LED_PIN, OUTPUT);
    pinMode(SKIP_LED_PIN, OUTPUT);
    pinMode(REPEAT_LED_PIN, OUTPUT);
    pinMode(SKIP_LED_PIN, OUTPUT);
}

// Enables interrupts on buttons
void enable_interrupts()
{
    enableInterrupt(LETTER_ORDERING_BUTTON_PIN, LETTER_ORDERING_BUTTON_PIN_handler, FALLING);
    enableInterrupt(LETTER_WAND_BUTTON_PIN, LETTER_WAND_BUTTON_PIN_handler, FALLING);
    enableInterrupt(NUMBER_ORDERING_BUTTON_PIN, NUMBER_ORDERING_BUTTON_PIN_handler, FALLING);
    enableInterrupt(NUMBER_WAND_BUTTON_PIN, NUMBER_WAND_BUTTON_PIN_handler, FALLING);

    enableInterrupt(HINT_BUTTON_PIN, HINT_BUTTON_PIN_handler, FALLING);
    enableInterrupt(END_GAME_BUTTON_PIN, END_GAME_BUTTON_PIN_handler, FALLING);
    enableInterrupt(REPEAT_BUTTON_PIN, REPEAT_BUTTON_PIN_handler, FALLING);
    enableInterrupt(SKIP_BUTTON_PIN, SKIP_BUTTON_PIN_handler, FALLING);

    enableInterrupt(RECALIBRATE_BUTTON, RECALIBRATE_BUTTON_handler, FALLING);
}

// Initialize both WS2811 LED strips (one for letters, one for numbers)
void initialize_led_strips()
{
    FastLED.addLeds<WS2811, LETTER_LED_STRIP_PIN, RGB>(letter_crgb_leds, NUM_LETTER_LEDS);
    FastLED.addLeds<WS2811, NUMBER_LED_STRIP_PIN, RGB>(number_crgb_leds, NUM_NUMBER_LEDS);
    for (int i = 0; i < NUM_LETTER_LEDS; i++)
    {
        letter_crgb_leds[i] = CRGB::Black;
    }
    FastLED.setBrightness(128);

    FastLED.show();
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
