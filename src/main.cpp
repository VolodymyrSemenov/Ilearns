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
    pinMode(HINT_LED_PIN, OUTPUT);
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

    enableInterrupt(LETTER_ORDERING_BUTTON_PIN, LETTER_ORDERING_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(NUMBER_ORDERING_BUTTON_PIN, NUMBER_ORDERING_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(LETTER_WAND_BUTTON_PIN, LETTER_WAND_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(NUMBER_WAND_BUTTON_PIN, NUMBER_WAND_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(HINT_BUTTON_PIN, HINT_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(END_GAME_BUTTON_PIN, END_GAME_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(REPEAT_BUTTON_PIN, REPEAT_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(SKIP_BUTTON_PIN, SKIP_BUTTON_PIN_handler_rising, RISING);
    enableInterrupt(RECALIBRATE_BUTTON, RECALIBRATE_BUTTON_handler_rising, RISING);
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

// Initialize reader, or halt if not found
void initialize_wand_reader()
{
    nfc.begin();
    nfc.setPassiveActivationRetries(0xFF);
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
        Serial.println("Didn't find PN532 board");
        fill_letters_solid(CRGB::Red);
        fill_numbers_solid(CRGB::Red);
        while (true)
        {
        }; // Halt
    }
    Serial.println("NFC reader initialized.");
}

void setup()
{
    Serial.begin(115200);
    illuminate_board_on_power_up();

    Serial.println("Setting pin modes");
    set_pin_modes();
    illuminate_setup_progress();

    Serial.println("Enable button interrupts");
    enable_interrupts();
    illuminate_setup_progress();

    Serial.println("Initializing LED strips");
    initialize_led_strips();
    illuminate_setup_progress();

    Serial.println("Initializing NFC reader");
    initialize_wand_reader();
    illuminate_setup_progress();

    Serial.println("EEPROM Initializing");
    populate_game_pieces_structure();
    illuminate_setup_progress();
    print_game_pieces();

    delay(250);
    fill_board_solid(CRGB::Green);
    delay(1500);

    Serial.println("Press a button to start a game.");
    illuminate_all_arcade_leds(HIGH);
}


bool flashing_active = true;
bool arcade_leds_on = false;
unsigned long last_flash_time = 0;
int pressed_button = -1;

const int flash_interval = 500; // ms

// --- Arcade Flashing Logic Function ---
void flash_game_arcade_leds() {
    if (!flashing_active) return;

    // Flash LEDs on/off every 500ms
    if (millis() - last_flash_time >= flash_interval) {
        last_flash_time = millis();
        if (arcade_leds_on) {
            illuminate_game_arcade_leds(LOW);
        } else {
            illuminate_game_arcade_leds(HIGH);
        }
        arcade_leds_on = !arcade_leds_on;
    }
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
        rainbow_dance(15);
        game_state = WAITING_STATE;
        delay(5000);
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
    if (game_state == WAITING_STATE)
    {
        rainbow_dance();
        flash_game_arcade_leds();
    }
}
