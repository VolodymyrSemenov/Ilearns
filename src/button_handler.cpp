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
void state_button_handler(int button_pressed)
{
    // Disregard mutliple button presses with 0.1s
    static int previous_press_time = 0;
    if ((millis() - previous_press_time) < 100)
    {
        return;
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
}

void letter_ordering_button_handler()
{
    // Handle letter ordering button press
    Serial.println("Letter Ordering Button Pressed");
    state_button_handler(letter_ordering_button);
}
void letter_wand_button_handler()
{
    // Handle letter wand button press
    Serial.println("Letter Wand Button Pressed");
    state_button_handler(letter_wand_button);
}
void number_ordering_button_handler()
{
    // Handle number ordering button press
    Serial.println("Number Ordering Button Pressed");
    state_button_handler(number_ordering_button);
}
void number_wand_button_handler()
{
    // Handle number wand button press
    Serial.println("Number Wand Button Pressed");
    state_button_handler(number_wand_button);
}
void hint_button_handler()
{
    // Handle hint button press
    Serial.println("Hint Button Pressed");
    state_button_handler(hint_button);
}
void end_game_button_handler()
{
    // Handle end game button press
    Serial.println("End Game Button Pressed");
    state_button_handler(end_game_button);
}
void repeat_button_handler()
{
    // Handle repeat button press
    Serial.println("Repeat Button Pressed");
    state_button_handler(repeat_button);
}
void skip_button_handler()
{
    // Handle skip button press
    Serial.println("Skip Button Pressed");
    state_button_handler(skip_button);
}
void recalibrate_button_handler()
{
    // Handle recalibrate button press
    Serial.println("Recalibrate button pressed");
    state_button_handler(recalibrate_button);
}
