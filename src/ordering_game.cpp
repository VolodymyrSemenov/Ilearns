#include <constants.h>
#include <structures.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <FastLED.h>
#include <illumination.h>
#include <order.h>

// void order_letter()
// {
//     int correct_selections = 0;
//     int max_correct = 26;

//     while (max_correct > correct_selections)
//     {
//         GamePieces currentList;
//         GamePiece currentLetter = currentList.letters[correct_selections];

//         flash_tile_location(currentLetter, CRGB::Yellow, 2);
//         illuminate_single_game_piece(currentLetter, CRGB::Yellow);

//         uint8_t uidLength;
//         uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
//         while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
//         {
//             delay(50); // avoid overwhelming the RFID reader
//         }

//         if (uids_match(uid, currentLetter.uid))
//         {
//             correct_selections += 1;

//             flash_tile_location(currentLetter, CRGB::Green, 2);
//             illuminate_single_game_piece(currentLetter, CRGB::Green);
//         }
//         else if (uid_is_uid_of_previous_gamepiece(correct_selections, currentList.letters, uid))
//         {
//             continue; // Don't flash the tile if it's already green
//         }
//         else
//         {
//             flash_tile_location(currentLetter, CRGB::Red, 2);
//             illuminate_single_game_piece(currentLetter, CRGB::Red);
//         }
//     }
// }