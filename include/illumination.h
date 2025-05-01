#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <structures.h>


void illuminate_single_game_piece(GamePiece game_piece, CRGB color);
void illuminate_range_of_letter_game_pieces(int first_indice, int last_indice, CRGB color);
void illuminate_range_of_number_game_pieces(int first_indice, int last_indice, CRGB color);

void illuminate_board_on_power_up();
void illuminate_setup_progress();

void flash_tile_location(GamePiece game_piece, CRGB color, int number_of_flashes, int delay_time = 250);
void rainbow_gradient_game_over(int delay_time = 15);
void rainbow_gradient_waiting(unsigned long delay_time = 50);

void illuminate_next_letter_tile_location(int tile_index, CRGB color);
void illuminate_next_number_tile_location(int tile_index, CRGB color);
void flash_next_letter_tile_location(int tile_index, CRGB color, int number_of_flashes, int delay_time = 250);
void flash_next_number_tile_location(int tile_index, CRGB color, int number_of_flashes, int delay_time = 250);

void fill_letters_solid(CRGB color);
void fill_numbers_solid(CRGB color);
void fill_board_solid(CRGB color);

void flash_board_letters(CRGB color, int number_of_flashes, int delay_time = 160);
void flash_board_numbers(CRGB color, int number_of_flashes, int delay_time = 160);
void flash_board_solid(CRGB color, int number_of_flashes, int delay_time = 200);

void illuminate_arcade_led(int button_number);
void deilluminate_arcade_led(int button_number);
void button_led_handler(int button_number);
void illuminate_game_arcade_leds(int pin_value);
void illuminate_all_arcade_leds(int pin_value);
void illuminate_active_game_arcade_led();


#endif