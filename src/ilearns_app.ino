#include <games/letter_ordering.ino>
#include <games/number_ordering.ino>
#include <games/find_letters_spoken.ino>
#include <games/find_letters_annunciation.ino>
#include <games/find_numbers.ino>

void setup(){
    // Turn on speakers
    // Turn on NFC reading library
    // Define game pieces

}


void loop(){

    // Determine Game State: 

    int button_number;
    bool button_pressed = check_button_pressed(button_number);

    // Button numbers can be 4 game buttons, 
    // the hint, repeat, skip, and fourth button on right side, 
    // the toggle switch next to a-z wand, and the reset button/switch

    if (button_pressed){
        if (button_number == 1){
            begin_game_letter_ordering();
        }
        else if (button_number == 2){
            // Call function
        }
        else if (button_number == 3){
            // Call function
        }
        else if (button_number == 4){
            // Call function
        }
        // Add more buttons here

        // Handle button presses in each of the functions for a hint, repeat, and skip button
    }

}

bool check_button_pressed(int &button_number){
    // Check if a button is pressed
    // If a button is pressed, return true and set button_number to the number of the button pressed
    // If no button is pressed, return false
    return false;
}


void assign_nfc_tags_to_game_pieces(){
    // Assign the NFC tags to the game pieces
}

