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
void LETTER_ORDERING_BUTTON_PIN_handler();
void LETTER_WAND_BUTTON_PIN_handler();
void NUMBER_ORDERING_BUTTON_PIN_handler();
void NUMBER_WAND_BUTTON_PIN_handler();
void HINT_BUTTON_PIN_handler();
void END_GAME_BUTTON_PIN_handler();
void REPEAT_BUTTON_PIN_handler();
void SKIP_BUTTON_PIN_handler();
void RECALIBRATE_BUTTON_handler();

#endif