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
volatile int utility_button_pressed = 0;

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

    pinMode(ENUNCIATION_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RECALIBRATE_BUTTON, INPUT_PULLUP);

    // Set led pins as outputs
    pinMode(LETTER_ORDER_LED_PIN, OUTPUT);
    pinMode(NUMBER_ORDERING_LED_PIN, OUTPUT);
    pinMode(LETTER_WAND_LED_PIN, OUTPUT);
    pinMode(NUMBER_WAND_LED_PIN, OUTPUT);

    pinMode(END_GAME_LED_PIN, OUTPUT);
    pinMode(SKIP_LED_PIN, OUTPUT);
    pinMode(REPEAT_LED_PIN, OUTPUT);
    pinMode(HINT_LED_PIN, OUTPUT);
}

// Enables interrupts on buttons
void enable_interrupts()
{
    enableInterrupt(LETTER_ORDERING_BUTTON_PIN, letter_ordering_button_handler, CHANGE);
    enableInterrupt(LETTER_WAND_BUTTON_PIN, letter_wand_button_handler, CHANGE);
    enableInterrupt(NUMBER_ORDERING_BUTTON_PIN, number_ordering_button_handler, CHANGE);
    enableInterrupt(NUMBER_WAND_BUTTON_PIN, number_wand_button_handler, CHANGE);
    enableInterrupt(HINT_BUTTON_PIN, hint_button_handler, CHANGE);
    enableInterrupt(END_GAME_BUTTON_PIN, end_game_button_handler, CHANGE);
    enableInterrupt(REPEAT_BUTTON_PIN, repeat_button_handler, CHANGE);
    enableInterrupt(SKIP_BUTTON_PIN, skip_button_handler, CHANGE);
    enableInterrupt(RECALIBRATE_BUTTON, recalibrate_button_handler, CHANGE);
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

void get_random_seed()
{
    unsigned long seed = 0;
    for (int i = 0; i < 4; i++)
    {
        seed ^= analogRead(i);
        delay(10);
    }
    seed ^= micros();
    randomSeed(seed);
}

void setup()
{
    Serial.begin(115200);
    illuminate_board_on_power_up();

    Serial.println("Initializing LED strips");
    initialize_led_strips();
    // illuminate_setup_progress();

    Serial.println("EEPROM Initializing");
    populate_game_pieces_structure();
    illuminate_setup_progress();

    Serial.println("Setting pin modes");
    set_pin_modes();
    illuminate_setup_progress();

    Serial.println("Enable button interrupts");
    enable_interrupts();
    illuminate_setup_progress();

    Serial.println("Initializing NFC reader");
    initialize_wand_reader();
    illuminate_setup_progress();

    print_game_pieces();

    delay(250);
    fill_board_solid(CRGB::Green);
    delay(1500);

    Serial.println("Press a button to start a game.");
    // illuminate_all_arcade_leds(HIGH);
}

bool flashing_active = true;
bool arcade_leds_on = false;
unsigned long last_flash_time = 0;
int pressed_button = -1;

constexpr int flash_interval = 1000; // ms

// --- Arcade Flashing Logic Function ---
void flash_game_arcade_leds()
{
    if (!flashing_active)
        return;

    // Flash LEDs on/off every 500ms
    if (millis() - last_flash_time >= flash_interval)
    {
        last_flash_time = millis();
        if (arcade_leds_on)
        {
            illuminate_game_arcade_leds(LOW);
        }
        else
        {
            illuminate_game_arcade_leds(HIGH);
        }
        arcade_leds_on = !arcade_leds_on;
    }
}

void loop()
{
    static int previous_state = game_state;
    if (game_state != previous_state)
    {
        Serial.print("Switching State: ");
        Serial.println(game_state);
        previous_state = game_state;
    }
    switch (game_state)
    {
    case GAME_OVER_STATE:
        rainbow_gradient_game_over();
        game_state = WAITING_STATE;
        break;

    case RECALIBRATING_STATE:
        Serial.println("Starting recalibration");
        recalibrate_game_pieces();
        game_state = WAITING_STATE;
        break;

    case LETTER_WAND_STATE:
        Serial.println("Starting letter wand game");
        begin_wand_game();
        game_state = GAME_OVER_STATE;
        break;

    case NUMBER_WAND_STATE:
        Serial.println("Starting number wand game");
        begin_wand_game();
        game_state = GAME_OVER_STATE;
        break;

    case LETTER_ORDERING_STATE:
        Serial.println("Starting letter ordering game");
        ordering_game();
        game_state = GAME_OVER_STATE;
        break;

    case NUMBER_ORDERING_STATE:
        Serial.println("Starting number ordering game");
        ordering_game();
        game_state = GAME_OVER_STATE;
        break;

    case WAITING_STATE:
        utility_button_pressed = 0;
        rainbow_gradient_waiting();
        illuminate_game_arcade_leds(HIGH);
        break;
    }
}
