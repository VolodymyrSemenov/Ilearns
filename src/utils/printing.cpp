#include <structures.h>


// Print a single game piece
void print_single_game_piece(GamePiece game_piece){
    Serial.print("\nCharacter: "); 
    Serial.println(game_piece.character);

    Serial.print("UID: "); 
    for (uint8_t j = 0; j < MAX_UID_LENGTH; j++) {
        Serial.print(game_piece.uid[j], HEX);
        Serial.print(" ");
    }
    Serial.println("");

    Serial.print("Positions: "); 
    for (int j = 0; j < WIDTH_PER_PIECE; j++) {
        Serial.print(game_piece.positions[j]); 
        Serial.print(" ");
    }
    Serial.println();

    Serial.print("Decoder Value: "); 
    Serial.println(game_piece.decoder_value);
    Serial.println();
}


// Print game pieces to Serial for debugging
void print_game_pieces() {

    Serial.println("\nLetter Game Pieces:");
    for (int i = 0; i < num_letters; i++) {
        print_single_game_piece(game_pieces.letters[i]);
    }

    Serial.println("\nNumber Game Pieces:");
    for (int i = 0; i < num_numbers; i++) {
        print_single_game_piece(game_pieces.numbers[i]);
    }
}