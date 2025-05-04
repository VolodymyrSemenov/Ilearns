#include <games.h>
#include <illumination.h>

void ordering_game()
{
    int skip = 0;
    int correct_selections = 0;
    int max_correct;
    GamePiece *selected_pieces;
    int hint_or_repeat_pressed = 0;
    if (game_state == LETTER_ORDERING_STATE)
    {
        fill_letters_solid(CRGB::White);
        fill_numbers_solid(CRGB::Black);
        selected_pieces = game_pieces.letters;
        max_correct = 26;
    }
    else
    {
        fill_numbers_solid(CRGB::White);
        fill_letters_solid(CRGB::Black);
        selected_pieces = game_pieces.numbers;
        max_correct = 21;
    }
    while (max_correct > correct_selections)
    {
        GamePiece current_game_piece = selected_pieces[correct_selections];

        illuminate_single_game_piece(current_game_piece, CRGB::Yellow);

        uint8_t uidLength;
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30) && !utility_button_pressed)
        {
            delay(20); // avoid overwhelming the RFID reader
        }
        
        if (utility_button_pressed)
        {
            switch (utility_button_pressed)
            {
            case END_GAME_BUTTON_PIN:
                Serial.println("End game button pressed. Exiting game.");
                correct_selections = max_correct; // End the game
                break;

            case SKIP_BUTTON_PIN:
                skip = 1;
                Serial.println("Skip button pressed.");
                break;

            default:
                hint_or_repeat_pressed = 1;
                break;
            }
            utility_button_pressed = 0; // Reset the button press
        }

        if (uids_match(uid, current_game_piece.uid) || skip)
        {
            Serial.println("matched");
            correct_selections += 1;
            illuminate_single_game_piece(current_game_piece, CRGB::Green);
            skip = 0;
        }
        else if (uid_is_uid_of_a_previous_gamepiece_in_list(correct_selections, selected_pieces, uid) || hint_or_repeat_pressed)
        {
            hint_or_repeat_pressed = 0;
            continue; // Don't flash the tile if it's already green, or hint/repeat pressed
        }
        else
        {
            flash_tile_location(current_game_piece, CRGB::Red, 1, 100);
            illuminate_single_game_piece(current_game_piece, CRGB::Yellow);
        }
    }
    Serial.println("Order Game Complete");
}
