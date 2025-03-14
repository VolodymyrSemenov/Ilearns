#include <../games/letter_ordering.ino>
#include <../games/number_ordering.ino>
#include <../games/find_letters_spoken.ino>
#include <../games/find_letters_annunciation.ino>
#include <../games/find_numbers.ino>

// Pins
const int letterOrderingBtn = 2;
const int numberOrderingBtn = 3;
const int findLettersBtn = 4;
const int findNumbersBtn = 5;
const int letterOrderingLED = 6;
const int numberOrderingLED = 7;
const int findLettersLED = 8;
const int findNumbersLED = 9;

void setup(){
    // Turn on speakers
    // Turn on NFC reading library
    // Turn on LEDs for buttons
    // Define game pieces
    
    Serial.begin(9600);
    pinMode(letterOrderingBtn, INPUT_PULLUP);
    pinMode(numberOrderingBtn, INPUT_PULLUP);
    pinMode(findLettersBtn, INPUT_PULLUP);
    pinMode(findNumbersBtn, INPUT_PULLUP);
    
    Serial.println("Press a button to start a game.");

}


void loop(){
    // Determine Game State: 

    int button_number;
    bool button_pressed = check_button_pressed(button_number);

    // Button numbers can be 4 game buttons, 
    // the hint, repeat, skip, and fourth button on right side, 
    // the toggle switch next to a-z wand, and the reset button/switch

    if (button_pressed) {
        switch (button_number) {
            case 2:
                Serial.println("Starting Letter Ordering Game...");
                // begin_game_letter_ordering();
                break;
            case 3:
                Serial.println("Starting Number Ordering Game...");
                // begin_game_number_ordering();
                break;
            case 4:
                Serial.println("Starting Find Letters Game...");
                // Check difficulty
                // begin_game_find_letters_spoken();
                // begin_game_find_letters_annunciation();
                break;
            case 5:
                Serial.println("Starting Find Numbers Game...");
                // begin_game_find_letters_annunciation();
                break;
            // Add more buttons here: hint, repeat, end game, and skip button
            default:
                Serial.println("Unknown button pressed.");
        }
    }

}

bool check_button_pressed(int &button_number){
    // Check if a button is pressed
    // If a button is pressed, return true and set button_number to the number of the button pressed
    // If no button is pressed, return false
    if (digitalRead(button1) == LOW) {
        button_number = 1;
        return true;
    } else if (digitalRead(button2) == LOW) {
        button_number = 2;
        return true;
    } else if (digitalRead(button3) == LOW) {
        button_number = 3;
        return true;
    } else if (digitalRead(button4) == LOW) {
        button_number = 4;
        return true;
    }
    return false;
}


void assign_nfc_tags_to_game_pieces(){
    // Assign the NFC tags to the game pieces
}

