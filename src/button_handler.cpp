#include <games.h>
#include <button_handler.h>
#include <constants.h>
#include <games.h>

// Illuminates a given arcade button led
void illuminate_arcade_led(int button_number)
{
    digitalWrite(button_number + ARCADE_LED_OFFSET, HIGH);
}

// Turns off a given arcade button led
void deilluminate_arcade_led(int button_number)
{
    digitalWrite(button_number + ARCADE_LED_OFFSET, LOW);
}

// Proper arcade button lighting during game_states
void button_led_handler(int button_number)
{
    // on falling edges (button presses)
    if (digitalRead(button_number) == 0)
    {
        illuminate_arcade_led(button_number);
    }

    // on rising edges (button release)
    else
    {
        if (button_number - OFFSET_BETWEEN_ARCADE_BUTTON_AND_GAMESTATE != game_state)
        {
            deilluminate_arcade_led(button_number);
        }
    }
}

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

    switch (button_pressed)
    {
    case letter_ordering_button:
        if (game_state == WAITING_STATE)
        {
            game_state = LETTER_ORDERING_STATE;
        }
        break;
    case letter_wand_button:
        if (game_state == WAITING_STATE)
        {
            if (digitalRead(annunciation_pin))
            {
                game_state = LETTER_WAND_STATE;
            }
            else
            {
                game_state = ENUNCIATION_STATE;
            }
        }
        break;
    case number_ordering_button:
        if (game_state == WAITING_STATE)
        {
            game_state = NUMBER_ORDERING_STATE;
        }
        break;
    case number_wand_button:
        if (game_state == WAITING_STATE)
        {
            game_state = NUMBER_WAND_STATE;
        }
        break;
    case hint_button:
        break;
    case repeat_button:
        break;
    case skip_button:
        break;
    case end_game_button:
        if (game_state != WAITING_STATE && game_state != RECALIBRATING_STATE)
        {
            game_state = GAME_OVER_STATE;
        }
        break;
    case recalibrate_button:
        game_state = RECALIBRATING_STATE;
    default:
        Serial.println("Error, this wasn't supposed to happen");
        break;
    }
    return true;
}

void letter_ordering_button_handler()
{
    if (state_button_handler(letter_ordering_button))
    {
        Serial.println("Letter Ordering Button Pressed");
    }
}
void letter_wand_button_handler()
{
    if (state_button_handler(letter_wand_button))
    {
        Serial.println("Letter Wand Button Pressed");
    }
}
void number_ordering_button_handler()
{
    if (state_button_handler(number_ordering_button))
    {
        Serial.println("Number Ordering Button Pressed");
    }
}
void number_wand_button_handler()
{
    if (state_button_handler(number_wand_button))
    {
        Serial.println("Number Wand Button Pressed");
    }
}
void hint_button_handler()
{
    if (state_button_handler(hint_button))
    {
        Serial.println("Hint Button Pressed");
    }
}
void end_game_button_handler()
{
    if (state_button_handler(end_game_button))
    {
        Serial.println("End Game Button Pressed");
    }
}
void repeat_button_handler()
{
    if (state_button_handler(repeat_button))
    {
        Serial.println("Repeat Button Pressed");
    }
}
void skip_button_handler()
{
    if (state_button_handler(skip_button))
    {
        Serial.println("Skip Button Pressed");
    }
}
void recalibrate_button_handler()
{
    if (state_button_handler(recalibrate_button))
    {
        Serial.println("Recalibrate button pressed");
    }
}
