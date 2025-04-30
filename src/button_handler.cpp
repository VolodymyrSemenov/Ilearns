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
        break;
    case REPEAT_BUTTON_PIN:
        break;
    case SKIP_BUTTON_PIN:
        break;
    case END_GAME_BUTTON_PIN:
        if (game_state != WAITING_STATE && game_state != RECALIBRATING_STATE)
        {
            game_state = GAME_OVER_STATE;
        }
        break;
    case RECALIBRATE_BUTTON:
        game_state = RECALIBRATING_STATE;
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
    }
}
void LETTER_WAND_BUTTON_PIN_handler()
{
    if (state_button_handler(LETTER_WAND_BUTTON_PIN))
    {
        Serial.println("Letter Wand Button Pressed");
    }
}
void NUMBER_ORDERING_BUTTON_PIN_handler()
{
    if (state_button_handler(NUMBER_ORDERING_BUTTON_PIN))
    {
        Serial.println("Number Ordering Button Pressed");
    }
}
void NUMBER_WAND_BUTTON_PIN_handler()
{
    if (state_button_handler(NUMBER_WAND_BUTTON_PIN))
    {
        Serial.println("Number Wand Button Pressed");
    }
}
void HINT_BUTTON_PIN_handler()
{
    if (state_button_handler(HINT_BUTTON_PIN))
    {
        Serial.println("Hint Button Pressed");
    }
}
void END_GAME_BUTTON_PIN_handler()
{
    if (state_button_handler(END_GAME_BUTTON_PIN))
    {
        Serial.println("End Game Button Pressed");
    }
}
void REPEAT_BUTTON_PIN_handler()
{
    if (state_button_handler(REPEAT_BUTTON_PIN))
    {
        Serial.println("Repeat Button Pressed");
    }
}
void SKIP_BUTTON_PIN_handler()
{
    if (state_button_handler(SKIP_BUTTON_PIN))
    {
        Serial.println("Skip Button Pressed");
    }
}
void RECALIBRATE_BUTTON_handler()
{
    if (state_button_handler(RECALIBRATE_BUTTON))
    {
        Serial.println("Recalibrate button pressed");
    }
}
