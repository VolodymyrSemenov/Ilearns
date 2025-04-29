#include <structures.h>





// Set the color of a game piece and display it on the LED strip
void illuminate_single_game_piece(GamePiece game_piece, CRGB color) {

    int indice = game_piece.decoder_value;
    Serial.println(indice);

    for (int i=game_piece.positions[0]; i<game_piece.positions[WIDTH_PER_PIECE-BLANK_LEDS_BETWEEN_PIECE]; i++){
        if (indice < num_letters){
            Serial.println("Letter");
            letter_crgb_leds[i] = color;
        }
        else{
            Serial.println("Number");
            number_crgb_leds[i] = color;
        }
        Serial.println(i);
    }
    FastLED.show(); // Display updated leds
}


void flash_tile_location(GamePiece game_piece, CRGB color, int number_of_flashes, int delay_time=250) {
    for (int i = 0; i < number_of_flashes; i++) {
        illuminate_single_game_piece(game_piece, color);
        delay(delay_time);
        illuminate_single_game_piece(game_piece, CRGB::Black);
        delay(delay_time);
    }
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


// For recalibrate
void illuminate_next_letter_tile_location(int tile_index, CRGB color) {
    int next_starting_position = FRONT_OF_LED_STRIP_OFFSET;
    // Serial.println("illuminate next Letter");
    // Serial.println(tile_index);
    // Serial.println(game_pieces.letters[tile_index - 1].positions[0]);

    if (tile_index > 0) {
        next_starting_position = game_pieces.letters[tile_index - 1].positions[0] + WIDTH_PER_PIECE;
    }

    Serial.println(next_starting_position);

    for (int i = next_starting_position; i < next_starting_position + WIDTH_PER_PIECE-BLANK_LEDS_BETWEEN_PIECE; i++) {
        letter_crgb_leds[i] = color;
    }
    FastLED.show();
}

// For recalibrate
void illuminate_next_number_tile_location(int tile_index, CRGB color) {
    int next_starting_position = FRONT_OF_LED_STRIP_OFFSET;
    if (tile_index > 0) {
        next_starting_position = game_pieces.numbers[tile_index - 1].positions[0] + WIDTH_PER_PIECE;
    }

    for (int i = next_starting_position; i < next_starting_position + WIDTH_PER_PIECE-BLANK_LEDS_BETWEEN_PIECE; i++) {
        number_crgb_leds[i] = color;
    }
    FastLED.show();
}