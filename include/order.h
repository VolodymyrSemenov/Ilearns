#ifndef order.h
#define order.h
#include <structures.h>


void flash_tile_location(GamePiece game_piece, CRGB color, int number_of_flashes, int delay_time=250);
void illuminate_single_game_piece(GamePiece game_piece, CRGB color);


bool uids_match(uint8_t uid1[], uint8_t uid2[]);
bool uid_is_uid_of_previous_gamepiece(int correct_selections, GamePiece random_game_pieces_list[], uint8_t uid[]);




#endif
