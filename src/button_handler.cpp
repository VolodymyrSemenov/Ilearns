#include <games.h>
#include <button_handler.h>
#include <constants.h>
#include <games.h>
#include <illumination.h>

// Begins proper game based on game_state and button pressed
// Returns false if button pressed too quickly
// Returns true if valid button press
bool state_button_handler(int button_pressed)
{

    // Disregard mutliple button presses with 0.1s
    static int previous_press_time = 0;
    if ((millis() - previous_press_time) < 100)
    {
        return false;
    }
    previous_press_time = millis();
    // button_led_handler(button_pressed);

    Serial.println("Button Pressed");
    int button_press_voltage = digitalRead(button_pressed);
    Serial.println(button_press_voltage);
    if (button_press_voltage == 0)
    {
        illuminate_arcade_led(button_pressed);
    }
    else{
        deilluminate_arcade_led(button_pressed);
    }

    illuminate_active_game_arcade_led();

    switch (button_pressed)
    {
    case LETTER_ORDERING_BUTTON_PIN:
        if (game_state == WAITING_STATE)
        {
            game_state = LETTER_ORDERING_STATE;
        }
        break;
    case LETTER_WAND_BUTTON_PIN:
        if (game_state == WAITING_STATE)
        {
            if (digitalRead(ENUNCIATION_PIN))
            {
                game_state = LETTER_WAND_STATE;
            }
            else
            {
                game_state = ENUNCIATION_STATE;
            }
        }
        break;
    case NUMBER_ORDERING_BUTTON_PIN:
        if (game_state == WAITING_STATE)
        {
            game_state = NUMBER_ORDERING_STATE;
        }
        break;
    case NUMBER_WAND_BUTTON_PIN:
        if (game_state == WAITING_STATE)
        {
            game_state = NUMBER_WAND_STATE;
        }
        break;

    case HINT_BUTTON_PIN:
        utility_button_pressed = HINT_BUTTON_PIN;
        break;

    case REPEAT_BUTTON_PIN:
        utility_button_pressed = REPEAT_BUTTON_PIN;
        break;

    case SKIP_BUTTON_PIN:
        utility_button_pressed = SKIP_BUTTON_PIN;
        break;

    case END_GAME_BUTTON_PIN:
        if (game_state != WAITING_STATE && game_state != RECALIBRATING_STATE)
        {
            game_state = GAME_OVER_STATE;
        }
        utility_button_pressed = END_GAME_BUTTON_PIN;
        break;
    case RECALIBRATE_BUTTON:
        game_state = RECALIBRATING_STATE;
        break;
        
    default:
        Serial.println("Error, this wasn't supposed to happen");
        break;
    }
    return true;
}

void LETTER_ORDERING_BUTTON_PIN_handler()
{
    if (state_button_handler(LETTER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Letter Ordering Button Pressed");
        // illuminate_arcade_led(LETTER_ORDERING_BUTTON_PIN);
    }
}
void LETTER_WAND_BUTTON_PIN_handler()
{
    if (state_button_handler(LETTER_WAND_BUTTON_PIN))
    {
        Serial.println("Letter Wand Button Pressed");
        // illuminate_arcade_led(LETTER_WAND_BUTTON_PIN);
    }
}
void NUMBER_ORDERING_BUTTON_PIN_handler()
{
    if (state_button_handler(NUMBER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Number Ordering Button Pressed");
        // illuminate_arcade_led(NUMBER_ORDERING_BUTTON_PIN);
    }
}
void NUMBER_WAND_BUTTON_PIN_handler()
{
    if (state_button_handler(NUMBER_WAND_BUTTON_PIN))
    {
        Serial.println("Number Wand Button Pressed");
        // illuminate_arcade_led(NUMBER_WAND_BUTTON_PIN);
    }
}
void HINT_BUTTON_PIN_handler()
{
    if (state_button_handler(HINT_BUTTON_PIN))
    {
        Serial.println("Hint Button Pressed");
        // illuminate_arcade_led(HINT_BUTTON_PIN);
    }
}
void END_GAME_BUTTON_PIN_handler()
{
    if (state_button_handler(END_GAME_BUTTON_PIN))
    {
        Serial.println("End Game Button Pressed");
        // illuminate_arcade_led(END_GAME_BUTTON_PIN);
    }
}
void REPEAT_BUTTON_PIN_handler()
{
    if (state_button_handler(REPEAT_BUTTON_PIN))
    {
        Serial.println("Repeat Button Pressed");
        // illuminate_arcade_led(REPEAT_BUTTON_PIN);
    }
}
void SKIP_BUTTON_PIN_handler()
{
    if (state_button_handler(SKIP_BUTTON_PIN))
    {
        Serial.println("Skip Button Pressed");
        // illuminate_arcade_led(SKIP_BUTTON_PIN);
    }
}
void RECALIBRATE_BUTTON_handler()
{
    if (state_button_handler(RECALIBRATE_BUTTON))
    {
        Serial.println("Recalibrate button pressed");
        // illuminate_arcade_led(RECALIBRATE_BUTTON);
    }
}






void LETTER_ORDERING_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(LETTER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Letter Ordering Button Released");
        // illuminate_arcade_led(LETTER_ORDERING_BUTTON_PIN);
    }
}
void NUMBER_ORDERING_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(NUMBER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Number Ordering Button Released");
        // illuminate_arcade_led(NUMBER_ORDERING_BUTTON_PIN);
    }
}
void LETTER_WAND_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(LETTER_WAND_BUTTON_PIN))
    {
        Serial.println("Letter Wand Button Released");
        // illuminate_arcade_led(LETTER_WAND_BUTTON_PIN);
    }
}
void NUMBER_WAND_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(NUMBER_WAND_BUTTON_PIN))
    {
        Serial.println("Number Wand Button Released");
        // illuminate_arcade_led(NUMBER_WAND_BUTTON_PIN);
    }
}
void HINT_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(HINT_BUTTON_PIN))
    {
        Serial.println("Hint Button Released");
        // illuminate_arcade_led(HINT_BUTTON_PIN);
    }
}
void END_GAME_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(END_GAME_BUTTON_PIN))
    {
        Serial.println("End Game Button Released");
        // illuminate_arcade_led(END_GAME_BUTTON_PIN);
    }
}
void REPEAT_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(REPEAT_BUTTON_PIN))
    {
        Serial.println("Repeat Button Released");
        // illuminate_arcade_led(REPEAT_BUTTON_PIN);
    }
}
void SKIP_BUTTON_PIN_handler_rising()
{
    if (state_button_handler(SKIP_BUTTON_PIN))
    {
        Serial.println("Skip Button Released");
        // illuminate_arcade_led(SKIP_BUTTON_PIN);
    }
}
void RECALIBRATE_BUTTON_handler_rising()
{
    if (state_button_handler(RECALIBRATE_BUTTON))
    {
        Serial.println("Recalibrate button pressed");
    }
}
