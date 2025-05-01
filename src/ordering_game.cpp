#include <constants.h>
#include <structures.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <FastLED.h>
#include <illumination.h>
#include <order.h>

void order_letter()
{
    int correct_selections = 0;
    int max_correct = 26;
    fill_board_solid(CRGB::White);

    while (max_correct > correct_selections)
    {
        GamePiece current_game_piece = game_pieces.letters[correct_selections];

        // flash_tile_location(current_game_piece, CRGB::Yellow, 2);
        illuminate_single_game_piece(current_game_piece, CRGB::Yellow);

        uint8_t uidLength;
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        if (uids_match(uid, current_game_piece.uid))
        {
            correct_selections += 1;
            // flash_tile_location(current_game_piece, CRGB::Green, 2);
            illuminate_single_game_piece(current_game_piece, CRGB::Green);
        }
        // else if (uid_is_uid_of_previous_gamepiece(correct_selections, random_game_pieces_list, uid))
        // {
        //     continue; // Don't flash the tile if it's already green
        // }
        else
        {
            flash_tile_location(current_game_piece, CRGB::Red, 1);
            illuminate_single_game_piece(current_game_piece, CRGB::Yellow);
            // illuminate_single_game_piece(current_game_piece, CRGB::Red);
        }
    }
}
