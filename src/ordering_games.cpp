#include <games.h>
#include <illumination.h>

void ordering_game()
{
    int correct_selections = 0;
    int max_correct;
    GamePiece *selected_pieces;
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

        // flash_tile_location(current_game_piece, CRGB::Yellow, 2);
        illuminate_single_game_piece(current_game_piece, CRGB::Yellow);

        uint8_t uidLength;
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
        {
            delay(20); // avoid overwhelming the RFID reader
        }

        if (uids_match(uid, current_game_piece.uid))
        {
            correct_selections += 1;
            // flash_tile_location(current_game_piece, CRGB::Green, 2);
            illuminate_single_game_piece(current_game_piece, CRGB::Green);
        }
        else if (uid_is_uid_of_previous_gamepiece(correct_selections, selected_pieces, uid))
        {
            continue; // Don't flash the tile if it's already green
        }
        else
        {
            flash_tile_location(current_game_piece, CRGB::Red, 1, 100);
            illuminate_single_game_piece(current_game_piece, CRGB::Yellow);
        }
    }
}
