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

    // Illuminate arcade leds
    int button_press_voltage = digitalRead(button_pressed);
    Serial.println("Button Pressed");
    Serial.println(button_press_voltage);

    if (button_press_voltage == 0)
    {
        illuminate_arcade_led(button_pressed);
    }
    else
    {
        deilluminate_arcade_led(button_pressed);
    }
    illuminate_active_game_arcade_led();

    // Change state based on button press
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
            if (digitalRead(ENUNCIATION_BUTTON_PIN))
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
        if (!digitalRead(HINT_BUTTON_PIN)){
            utility_button_pressed = HINT_BUTTON_PIN;
        }
        break;

    case REPEAT_BUTTON_PIN:
        if (!digitalRead(REPEAT_BUTTON_PIN)){
            utility_button_pressed = REPEAT_BUTTON_PIN;
        }
        break;

    case SKIP_BUTTON_PIN:
        Serial.println("Skip button pressed CASE");
        Serial.println(digitalRead(SKIP_BUTTON_PIN));
        if (!digitalRead(SKIP_BUTTON_PIN)){
            utility_button_pressed = SKIP_BUTTON_PIN;
        }
        break;

    case END_GAME_BUTTON_PIN:
        if (!digitalRead(END_GAME_BUTTON_PIN)){
            utility_button_pressed = END_GAME_BUTTON_PIN;
        }

        if (game_state != WAITING_STATE && game_state != RECALIBRATING_STATE)
        {
            game_state = GAME_OVER_STATE;
        }
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

void letter_ordering_button_handler()
{
    if (state_button_handler(LETTER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Letter Ordering Button Pressed");
    }
}
void letter_wand_button_handler()
{
    if (state_button_handler(LETTER_WAND_BUTTON_PIN))
    {
        Serial.println("Letter Wand Button Pressed");
    }
}
void number_ordering_button_handler()
{
    if (state_button_handler(NUMBER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Number Ordering Button Pressed");
    }
}
void number_wand_button_handler()
{
    if (state_button_handler(NUMBER_WAND_BUTTON_PIN))
    {
        Serial.println("Number Wand Button Pressed");
    }
}
void hint_button_handler()
{
    if (state_button_handler(HINT_BUTTON_PIN))
    {
        Serial.println("Hint Button Pressed");
    }
}
void end_game_button_handler()
{
    if (state_button_handler(END_GAME_BUTTON_PIN))
    {
        Serial.println("End Game Button Pressed");
    }
}
void repeat_button_handler()
{
    if (state_button_handler(REPEAT_BUTTON_PIN))
    {
        Serial.println("Repeat Button Pressed");
    }
}
void skip_button_handler()
{
    if (state_button_handler(SKIP_BUTTON_PIN))
    {
        Serial.println("Skip Button Pressed");
    }
}
void recalibrate_button_handler()
{
    if (state_button_handler(RECALIBRATE_BUTTON))
    {
        Serial.println("Recalibrate button pressed");
    }
}
