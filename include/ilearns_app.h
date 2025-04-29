#ifndef ILEARNS_APP_H
#define ILEARNS_APP_H

#include <structures.h>

void recalibrate_game_pieces();
void illuminate_next_letter_tile_location(int tile_index, CRGB color);
void illuminate_single_game_piece(GamePiece game_piece, CRGB color);


#endif