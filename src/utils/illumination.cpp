#include <structures.h>


int base_letter_setup_indice = 0;
int base_number_setup_indice = 0;

void fill_letters_solid(CRGB color)
{
    for (int i = OFFSET_BLANK_WS2811_LEDS; i < NUM_LETTER_LEDS; i += 3)
    {
        letter_crgb_leds[i] = color;
        letter_crgb_leds[i + 1] = color;
        letter_crgb_leds[i + 2] = CRGB::Black;
    }
    FastLED.show();
}

void fill_numbers_solid(CRGB color)
{
    for (int i = OFFSET_BLANK_WS2811_LEDS; i < NUM_NUMBER_LEDS; i += 3)
    {
        number_crgb_leds[i] = color;
        number_crgb_leds[i + 1] = color;
        number_crgb_leds[i + 2] = CRGB::Black;
    }
    FastLED.show();
}

void fill_board_solid(CRGB color)
{
    fill_letters_solid(color);
    fill_numbers_solid(color);
}

// Set the color of a game piece and display it on the LED strip
void illuminate_single_game_piece(GamePiece game_piece, CRGB color)
{

    int indice = game_piece.decoder_value;
    Serial.println(indice);

    for (int i = game_piece.positions[0]; i < game_piece.positions[WIDTH_PER_PIECE - BLANK_LEDS_BETWEEN_PIECE]; i++)
    {
        if (indice < NUM_LETTERS)
        {
            Serial.println("Letter");
            letter_crgb_leds[i] = color;
        }
        else
        {
            Serial.println("Number");
            number_crgb_leds[i] = color;
        }
        Serial.println(i);
    }
    FastLED.show(); // Display updated leds
}


void illuminate_range_of_letter_game_pieces(int first_indice, int last_indice, CRGB color)
{
    for (int i = first_indice; i < last_indice; i++)
    {
        illuminate_single_game_piece(game_pieces.letters[i], color);
    }
}


void illuminate_range_of_number_game_pieces(int first_indice, int last_indice, CRGB color)
{
    for (int i = first_indice; i < last_indice; i++)
    {
        illuminate_single_game_piece(game_pieces.numbers[i], color);
    }
}



void illuminate_board_on_power_up(){
    fill_board_solid(CRGB::Black);
    fill_board_solid(CRGB::Purple);
    delay(500);
    fill_board_solid(CRGB::Black);
}

void illuminate_setup_progress()
{
    Serial.println("illuminate setup progress");
    int base_letter_increment = 6;
    int base_number_increment = 5;

    if (base_letter_setup_indice >=18){
        base_letter_increment = 8;
        base_number_increment = 6;
    }

    illuminate_range_of_letter_game_pieces(base_letter_setup_indice, base_letter_setup_indice+base_letter_increment, CRGB::Purple);
    illuminate_range_of_number_game_pieces(base_number_setup_indice, base_number_setup_indice+base_number_increment, CRGB::Purple);
    
    base_letter_setup_indice += base_letter_increment;
    base_number_setup_indice += base_number_increment;
    delay(500);
}

void flash_tile_location(GamePiece game_piece, CRGB color, int number_of_flashes, int delay_time = 250)
{
    for (int i = 0; i < number_of_flashes; i++)
    {
        illuminate_single_game_piece(game_piece, color);
        delay(delay_time);
        illuminate_single_game_piece(game_piece, CRGB::Black);
        delay(delay_time);
    }
}




void rainbow_dance(unsigned long delay_time = 50)
{
    static uint8_t hue = 0;
    static unsigned long last_update = 0;

    if (millis() - last_update < delay_time)
        return;

    last_update = millis();
    uint8_t currentHue = hue;
    int hue_increase = 16;

    for (int i = OFFSET_BLANK_WS2811_LEDS; i < NUM_LETTER_LEDS; i += 3)
    {
        letter_crgb_leds[i] = CHSV(currentHue, 255, 255);
        letter_crgb_leds[i + 1] = CHSV(currentHue + 8, 255, 255);
        letter_crgb_leds[i + 2] = CRGB::Black;
        currentHue += hue_increase;
    }

    currentHue = hue;
    for (int i = OFFSET_BLANK_WS2811_LEDS; i < NUM_NUMBER_LEDS; i += 3)
    {
        number_crgb_leds[i] = CHSV(currentHue, 255, 255);
        number_crgb_leds[i + 1] = CHSV(currentHue + 8, 255, 255);
        number_crgb_leds[i + 2] = CRGB::Black;
        currentHue -= hue_increase;
    }

    FastLED.show();
    hue += 5;
}

// Subroutine to start a rainbow LED dance when game_over is true.
// New subroutine to start a rainbow LED dance for 5 seconds when game_over is true.
// void startLEDRainbowDance() {
//     unsigned long startTime = millis(); // Record the start time

//     // Run a rainbow pattern for 5 seconds.
//     while (millis() - startTime < 5000) {
//         static uint8_t hue = 0;
//         fill_rainbow(letter_crgb_leds, NUM_LETTER_LEDS, hue, 8);
//         fill_rainbow(number_crgb_leds, NUM_NUMBER_LEDS, hue, 8);
//         FastLED.show();
//         hue += 5; // Gradually shift colors
//         delay(50);
//     }

//     // After 5 seconds, clear the LEDs
//     FastLED.clear();
//     FastLED.show();
// }

void startLEDRainbowDance(int delay_time = 15)
{
    unsigned long startTime = millis(); // Record the start time
    static uint8_t hue = 0;

    // Run a rainbow pattern for 5 seconds.
    while (millis() - startTime < 5000)
    {
        uint8_t currentHue = hue;
        int hue_increase = 16;

        // Animate only active letter LEDs (2-on, 1-off pattern)
        for (int i = OFFSET_BLANK_WS2811_LEDS; i < NUM_LETTER_LEDS; i += 3)
        {
            letter_crgb_leds[i] = CHSV(currentHue, 255, 255);
            letter_crgb_leds[i + 1] = CHSV(currentHue + 8, 255, 255);
            letter_crgb_leds[i + 2] = CRGB::Black;
            currentHue += hue_increase;
        }

        // Animate only active number LEDs (same pattern)
        currentHue = hue;
        for (int i = OFFSET_BLANK_WS2811_LEDS; i < NUM_NUMBER_LEDS; i += 3)
        {
            number_crgb_leds[i] = CHSV(currentHue, 255, 255);
            number_crgb_leds[i + 1] = CHSV(currentHue + 8, 255, 255);
            number_crgb_leds[i + 2] = CRGB::Black;
            currentHue -= hue_increase;
        }

        FastLED.show();
        hue += 5; // Shift hue for next loop
        delay(delay_time);
    }

    // After 5 seconds, clear the LEDs
    FastLED.clear();
    FastLED.show();
}

// For recalibrate
void illuminate_next_letter_tile_location(int tile_index, CRGB color)
{
    int next_starting_position = OFFSET_BLANK_WS2811_LEDS;
    // Serial.println("illuminate next Letter");
    // Serial.println(tile_index);
    // Serial.println(game_pieces.letters[tile_index - 1].positions[0]);

    if (tile_index > 0)
    {
        next_starting_position = game_pieces.letters[tile_index - 1].positions[0] + WIDTH_PER_PIECE;
    }

    Serial.println(next_starting_position);

    for (int i = next_starting_position; i < next_starting_position + WIDTH_PER_PIECE - BLANK_LEDS_BETWEEN_PIECE; i++)
    {
        letter_crgb_leds[i] = color;
    }
    FastLED.show();
}

// For recalibrate
void illuminate_next_number_tile_location(int tile_index, CRGB color)
{
    int next_starting_position = OFFSET_BLANK_WS2811_LEDS;
    if (tile_index > 0)
    {
        next_starting_position = game_pieces.numbers[tile_index - 1].positions[0] + WIDTH_PER_PIECE;
    }

    for (int i = next_starting_position; i < next_starting_position + WIDTH_PER_PIECE - BLANK_LEDS_BETWEEN_PIECE; i++)
    {
        number_crgb_leds[i] = color;
    }
    FastLED.show();
}



// ARCADE BUTTONS

// Illuminates a given arcade button led
void illuminate_arcade_led(int button_number)
{   
    Serial.println("Illuminating arcade led");
    Serial.println(button_number+OFFSET_ARCADE_BUTTON_TO_ARCADE_LED);
    digitalWrite(button_number + OFFSET_ARCADE_BUTTON_TO_ARCADE_LED, HIGH);
    delay(1000);
}

// Turns off a given arcade button led
void deilluminate_arcade_led(int button_number)
{
    Serial.println("De illuminating arcade led");
    Serial.println(button_number+OFFSET_ARCADE_BUTTON_TO_ARCADE_LED);
    digitalWrite(button_number + OFFSET_ARCADE_BUTTON_TO_ARCADE_LED, LOW);
}

// Proper arcade button lighting during game_states
void button_led_handler(int button_number)
{
    Serial.println("Button LED Handler");
    Serial.println(button_number);
    Serial.println("Game state");
    Serial.println(game_state);
    Serial.println(digitalRead(button_number));
    // on falling edges (button presses)
    if (digitalRead(button_number) == FALLING)
    {
        Serial.println("Button Pressed IF"); 
        illuminate_arcade_led(button_number);
    }

    // on rising edges (button release)
    else
    {
        Serial.println("De illuminate");
        deilluminate_arcade_led(button_number);

        // Serial.println("Button Pressed Else");
        // if (button_number - OFFSET_GAMESTATE_TO_ARCADE_BUTTON != game_state)
        // {
        //     Serial.println("De illuminate");
        //     deilluminate_arcade_led(button_number);
        // }
    }
}

void illuminate_game_arcade_leds(int pin_value)
{
    digitalWrite(letter_ordering_led, pin_value);
    digitalWrite(number_ordering_led, pin_value);
    digitalWrite(letter_wand_led, pin_value);
    digitalWrite(number_wand_led, pin_value);
}

void illuminate_all_arcade_leds(int pin_value)
{
    digitalWrite(letter_ordering_led, pin_value);
    digitalWrite(number_ordering_led, pin_value);
    digitalWrite(letter_wand_led, pin_value);
    digitalWrite(number_wand_led, pin_value);

    digitalWrite(END_GAME_LED_PIN, pin_value);
    digitalWrite(SKIP_LED_PIN, pin_value);
    digitalWrite(REPEAT_LED_PIN, pin_value);
    digitalWrite(HINT_LED_PIN, pin_value);
}

void illuminate_active_game_arcade_led(){
    // illuminate_all_arcade_leds(LOW);
    Serial.println("properly illuminate arcade leds");
    Serial.println("Game state");
    Serial.println(game_state);
    if (game_state>=1 && game_state<=5){
        int button_to_illuminate = game_state + OFFSET_GAMESTATE_TO_ARCADE_BUTTON;
        if (game_state==5){
            button_to_illuminate = LETTER_WAND_BUTTON_PIN;
        }
        Serial.println("Button to illuminate");
        Serial.println(button_to_illuminate);
        illuminate_game_arcade_leds(LOW);
        illuminate_arcade_led(button_to_illuminate);
    }
}