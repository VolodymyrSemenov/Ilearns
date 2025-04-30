#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

// LED control
void illuminate_button_led(int button_number);
void deilluminate_button_led(int button_number);
void button_led_handler(int button_number);

// Game state handling
bool state_button_handler(int button_pressed);

// Button press handlers
void letter_ordering_button_handler();
void letter_wand_button_handler();
void number_ordering_button_handler();
void number_wand_button_handler();
void hint_game_button_handler();
void end_game_button_handler();
void repeat_button_handler();
void skip_button_handler();
void recalibrate_button_handler();

void letter_ordering_button_handler_rising();
void number_ordering_button_handler_rising();
void letter_wand_button_handler_rising();
void number_wand_button_handler_rising();
void hint_game_button_handler_rising();
void end_game_button_handler_rising();
void repeat_button_handler_rising();
void skip_button_handler_rising();
void recalibrate_button_handler_rising();

#endif