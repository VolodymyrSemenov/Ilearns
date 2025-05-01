#ifndef GAMES_H
#define GAMES_H

#include <structures.h>

void begin_wand_game(void);
void ordering_game(void);
bool uid_is_uid_of_previous_gamepiece(int correct_selections, GamePiece random_game_pieces_list[], uint8_t uid[]);
bool uids_match(uint8_t uid1[], uint8_t uid2[]);

#endif