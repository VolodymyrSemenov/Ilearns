/**************************************************************************
 * Find Numbers Game
 **************************************************************************/

// #include <audio_config.ino>
#include <constants.h>
#include <structures.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <FastLED.h>
#include <ilearns_app.h>

#include <illumination.h>

// Return true if the game piece is in a list of game pieces
bool game_piece_is_in_list(GamePiece game_piece, GamePiece list_of_game_pieces[]) {
  for (int i = 0; i < sizeof(list_of_game_pieces)/sizeof(GamePiece); i++) {
    if (list_of_game_pieces[i].character == game_piece.character) {
      return true;
    }
  }
  return false;
}

// Get a random game piece from the list that's not in the random game pieces list
GamePiece get_unique_random_gamepiece(int max_size, GamePiece game_piece_list[], GamePiece random_game_pieces_list[]) {
  int random_index = random(0, max_size);

  while (game_piece_is_in_list(game_piece_list[random_index], random_game_pieces_list)) {
    random_index = random(0, max_size);
  } 
  return game_piece_list[random_index];
}



String get_random_number() {
  int index = random(0, num_numbers);
  return String(game_pieces.numbers[index].character);
}


bool uids_match(uint8_t uid1[], uint8_t uid2[]) {
  for (uint8_t i = 0; i < MAX_UID_LENGTH; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}



void send_serial_audio_command(GamePiece game_piece) {
  String game_piece_character_string;

  if (game_state == 4){
    // Convert number as bytes to String
    game_piece_character_string = String((char)game_piece.character);
  }
  else{
    game_piece_character_string = String(game_piece.character);
  }
  Serial.println(game_piece_character_string);

  // Play audio to giga over serial
  // Serial1.print(",");      // Separator
  Serial1.print(game_state); // Send game state
  Serial1.println(game_piece_character_string);   // Send character first
}


// Given a uid, return its gamepiece
GamePiece get_gamepiece_by_uid(uint8_t uid[]) {
  for (int i = 0; i < num_letters; i++) {
    if (uids_match(game_pieces.letters[i].uid, uid)) {
      return game_pieces.letters[i];
    }
  }
  for (int i = 0; i < num_numbers; i++) {
    if (uids_match(game_pieces.numbers[i].uid, uid)) {
      return game_pieces.numbers[i];
    }
  }
  return GamePiece(); // Return an empty GamePiece if not found
}


bool uid_is_uid_of_previous_gamepiece(int correct_selections, GamePiece random_game_pieces_list[], uint8_t uid[]) {
  for (int i=0; i<correct_selections; i++){
    if (uids_match(uid, random_game_pieces_list[i].uid)){
      return true;
    }
  }
  return false;
}



void begin_wand_game() {
  bool game_over = false;
  while (!game_over){
    int correct_selections = 0;
    const int max_correct = 5;
    GamePiece random_game_pieces_list[max_correct];

    Serial.println("Starting Find Numbers Game...");

    for (int i=0; i<max_correct; i++){
      if (game_state == NUMBER_ORDERING_STATE){
        random_game_pieces_list[i] = get_unique_random_gamepiece(max_correct, game_pieces.numbers, random_game_pieces_list);
        fill_letters_solid(CRGB::Yellow);
      }
      else{
        random_game_pieces_list[i] = get_unique_random_gamepiece(max_correct, game_pieces.letters, random_game_pieces_list);
        fill_numbers_solid(CRGB::Yellow);
      }

    }
    
    while (correct_selections < max_correct) {
      GamePiece current_game_piece = random_game_pieces_list[correct_selections];

      Serial.print("Find the following tile: ");
      Serial.println(current_game_piece.character);

      send_serial_audio_command(current_game_piece);

      uint8_t uidLength;
      uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
      while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30)) {
          delay(50);  // avoid overwhelming the RFID reader
      }

      if (uids_match(uid, current_game_piece.uid)) {
        flash_tile_location(current_game_piece, CRGB::Green, 2);
        illuminate_single_game_piece(current_game_piece, CRGB::Green);
        correct_selections++;
      }

      // Don't flash the tile if it's already green
      else if (uid_is_uid_of_previous_gamepiece(correct_selections, random_game_pieces_list, uid)){
        continue;
      }

      else{
        GamePiece incorrect_gamepiece = get_gamepiece_by_uid(uid);
        flash_tile_location(incorrect_gamepiece, CRGB::Red, 2);
        Serial.println("Incorrect selection. Try again!");
      }
    }
    game_over = true;
    Serial.println("Game complete: 5 correct selections achieved!");
  }
  game_state = GAME_OVER_STATE;
}
