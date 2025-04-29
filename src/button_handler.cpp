// #include <button_handler.h>
#include <games.h>
#include <button_handler.h>
#include <constants.h>
#include <games.h>
#include <ilearns_app.h>

// game state:
// 0 - waiting for a game to start
// 1 - letter ordering game
// 2 - letter wand game
// 3 - number ordering game
// 4 - number wand game

// 5 - letter wand game spoken





// Illuminates a given arcade button led
void illuminate_arcade_led(int button_number){
    digitalWrite(button_number+ARCADE_LED_OFFSET, HIGH);
}

// Turns off a given arcade button led
void deilluminate_arcade_led(int button_number){
    digitalWrite(button_number+ARCADE_LED_OFFSET, LOW);
}


// Proper arcade button lighting during game_states
void button_led_handler(int button_number){

    // on falling edges (button presses)
    if (digitalRead(button_number) == 0){
        illuminate_arcade_led(button_number);
    }

    // on rising edges (button release)
    else{
        if (button_number-OFFSET_BETWEEN_ARCADE_BUTTON_AND_GAMESTATE != game_state){
            deilluminate_arcade_led(button_number);
        }
    }
}


// Begins proper game based on game_state and button pressed
void state_button_handler(int button_pressed) {
    if (game_state == 0){
        game_over = 0;
        switch (button_pressed){
            case letter_wand_button:
                // Easy (spoken) mode for switch flipped to 0; i.e. pull up resistor makes pin high
                if (annunciation_pin){
                    break;
                    // begin_game_find_letters_spoken();
                }
                else{
                    break;
                    // begin_game_find_letters_annunciation();
                }

            case number_wand_button:
                begin_wand_game();

            case recalibrate_button:
                recalibrate_game_pieces();
                break;
                

            default:
                break;
        }
    }

    else if (button_pressed == end_game_button){
        game_state = 0;
        game_over = 1;
    }

    // else if ()
    
    
    // switch(game_state) {
    //     case 0:
            

    //     case :
    //         break;

    // }

    // switch (button_pressed) {
    //     case letter_ordering_button:
    //         // Handle letter ordering button press
    //         Serial.println("Letter Ordering Button Pressed");
    //         // if (game_state
    //         break;
    //     case letter_wand_button:
    //         // Handle letter wand button press
    //         Serial.println("Letter Wand Button Pressed");
    //         break;
    //     case number_ordering_button:
    //         // Handle number ordering button press
    //         Serial.println("Number Ordering Button Pressed");
    //         break;
    //     case number_wand_button:
    //         // Handle number wand button press
    //         Serial.println("Number Wand Button Pressed");
    //         break;
    //     case hint_button:
    //         // Handle hint button press
    //         Serial.println("Hint Button Pressed");
    //         break;
    //     case end_game_button:
    //         // Handle end game button press
    //         Serial.println("End Game Button Pressed");
    //         break;
    //     case repeat_button:
    //         // Handle repeat button press
    //         Serial.println("Repeat Button Pressed");
    //         break;
    //     case skip_button:
    //         // Handle skip button press
    //         Serial.println("Skip Button Pressed");
    //         break;
    // }





    // if game_state == 0 {
    //     // Game not started
    //     Serial.println("Game not started");
    // } else if game_state == 1 {
    //     // Letter ordering game
    //     Serial.println("Letter Ordering Game");
    // } else if game_state == 2 {
    //     // Letter wand game spoken
    //     Serial.println("Letter Wand Game Spoken");
    // } else if game_state == 3 {
    //     // Letter wand game annunciated
    //     Serial.println("Letter Wand Game Annunciated");
    // } else if game_state == 4 {
    //     // Number ordering game
    //     Serial.println("Number Ordering Game");
    // } else if game_state == 5 {
    //     // Number wand game spoken
    //     Serial.println("Number Wand Game Spoken");
    // }
}


void letter_ordering_button_handler() {
    // Handle letter ordering button press
    Serial.println("Letter Ordering Button Pressed");
    state_button_handler(letter_ordering_button);
}
void letter_wand_button_handler() {
    // Handle letter wand button press
    Serial.println("Letter Wand Button Pressed");
    state_button_handler(letter_wand_button);
}
void number_ordering_button_handler() {
    // Handle number ordering button press
    Serial.println("Number Ordering Button Pressed");
    state_button_handler(number_ordering_button);
}
void number_wand_button_handler() {
    // Handle number wand button press
    Serial.println("Number Wand Button Pressed");
    state_button_handler(number_wand_button);
}
void hint_button_handler() {
    // Handle hint button press
    Serial.println("Hint Button Pressed");
    state_button_handler(hint_button);
}
void end_game_button_handler() {
    // Handle end game button press
    Serial.println("End Game Button Pressed");
    state_button_handler(end_game_button);
}
void repeat_button_handler() {
    // Handle repeat button press
    Serial.println("Repeat Button Pressed");
    state_button_handler(repeat_button);
}
void skip_button_handler() {
    // Handle skip button press
    Serial.println("Skip Button Pressed");
    state_button_handler(skip_button);
}
void recalibrate_button_handler() {
    // Handle recalibrate button press
    Serial.println("Recalibrate button pressed");
    state_button_handler(recalibrate_button);
}
