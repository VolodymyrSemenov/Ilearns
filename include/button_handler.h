#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H


// Game state handling
bool state_button_handler(int button_pressed);

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

#endif