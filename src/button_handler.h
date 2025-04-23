#pragma once
#include <Arduino.h>
#include "constants.h"  // make sure this contains pin and state constants

// External shared variables
extern int game_state;
extern bool game_over;
extern const int annunciation_pin;

extern const int letter_ordering_button;
extern const int letter_wand_button;
extern const int number_ordering_button;
extern const int number_wand_button;
extern const int hint_button;
extern const int end_game_button;
extern const int repeat_button;
extern const int skip_button;
extern const int recalibrate_button;

// LED control
void illuminate_button_led(int button_number);
void deilluminate_button_led(int button_number);
void button_led_handler(int button_number);

// Game state handling
void state_button_handler(int button_pressed);

// Button press handlers
void letter_ordering_button_handler();
void letter_wand_button_handler();
void number_ordering_button_handler();
void number_wand_button_handler();
void hint_button_handler();
void end_game_button_handler();
void repeat_button_handler();
void skip_button_handler();
void recalibrate_button_handler();
