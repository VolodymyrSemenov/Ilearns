#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <structures.h>

// Set the color of a game piece and display it on the LED strip
void illuminate_single_game_piece(GamePiece game_piece, CRGB color);

void flash_tile_location(GamePiece game_piece, CRGB color, int number_of_flashes, int delay_time = 250);

// Subroutine to start a rainbow LED dance when game_over is true.
// New subroutine to start a rainbow LED dance for 5 seconds when game_over is true.
void startLEDRainbowDance();

// For recalibrate
void illuminate_next_letter_tile_location(int tile_index, CRGB color);

// For recalibrate
void illuminate_next_number_tile_location(int tile_index, CRGB color);

void fill_letters_solid(CRGB color);
void fill_numbers_solid(CRGB color);

#endif