#include <games.h>
#include <illumination.h>
#include <printing.h>


int hint_is_active = 0;
constexpr int max_hints = 3; // plus the correct one
GamePiece hint_game_pieces_list[max_hints+1];

// Return true if the game piece is in a list of game pieces
bool game_piece_is_in_list(GamePiece game_piece, GamePiece list_of_game_pieces[], int size_of_list_of_game_pieces)
{
  for (int i = 0; i < size_of_list_of_game_pieces; i++)
  {
    if (list_of_game_pieces[i].character == game_piece.character)
    {
      return true;
    }
  }
  return false;
}

// Get a random game piece from the list that's not in the random game pieces list
GamePiece get_unique_random_gamepiece(int max_random_value, GamePiece game_piece_list[], GamePiece random_game_pieces_list[], int size_of_list_of_game_pieces)
{
  int random_index = random(0, max_random_value);

  while (game_piece_is_in_list(game_piece_list[random_index], random_game_pieces_list, size_of_list_of_game_pieces) ||
       (hint_is_active && game_piece_is_in_list(game_piece_list[random_index], hint_game_pieces_list, max_hints)))
  {
    random_index = random(0, max_random_value);
  }
  return game_piece_list[random_index];
}

bool uids_match(uint8_t uid1[], uint8_t uid2[])
{
  for (uint8_t i = 0; i < MAX_UID_LENGTH; i++)
  {
    if (uid1[i] != uid2[i])
    {
      return false;
    }
  }
  return true;
}

void send_serial_audio_command(GamePiece game_piece)
{

  byte byte_to_send;

  if (game_state == LETTER_WAND_STATE){
    byte_to_send = game_piece.character - 'a'; // 'a' is 97 in ASCII
  }

  else if (game_state == ENUNCIATION_STATE){
    byte_to_send = game_piece.character - 'a' + 26; // 'a' is 97 in ASCII
  }

  else if (game_state == NUMBER_WAND_STATE){
    byte_to_send = game_piece.character + 52; // 0-20
  }
  
  else if (game_state == GAME_OVER_STATE) {
    byte_to_send = 73;
  }
  Serial.print("Sending byte: ");
  Serial.println(byte_to_send, DEC);
  Serial2.write(byte_to_send);
}

// Given a uid, return its gamepiece
GamePiece get_gamepiece_by_uid(uint8_t uid[])
{
  for (int i = 0; i < NUM_LETTERS; i++)
  {
    if (uids_match(game_pieces.letters[i].uid, uid))
    {
      return game_pieces.letters[i];
    }
  }
  for (int i = 0; i < NUM_NUMBERS; i++)
  {
    if (uids_match(game_pieces.numbers[i].uid, uid))
    {
      return game_pieces.numbers[i];
    }
  }
  return GamePiece(); // Return an empty GamePiece if not found
}

bool uid_is_uid_of_a_previous_gamepiece_in_list(int correct_selections, GamePiece random_game_pieces_list[], uint8_t uid[])
{
  for (int i = 0; i < correct_selections; i++)
  {
    if (uids_match(uid, random_game_pieces_list[i].uid))
    {
      return true;
    }
  }
  return false;
}

void generate_random_seed()
{
  unsigned long seed = 0;
  for (int i = 0; i < 4; i++)
  {
    seed ^= analogRead(i);
    delay(10);
  }
  seed ^= micros();
  randomSeed(seed);
}

void begin_wand_game()
{
  int correct_selections = 0;
  constexpr int max_correct = 5;
  GamePiece random_game_pieces_list[max_correct];
  
  int repeat = 0;
  int skip = 0;
  generate_random_seed();

  illuminate_active_game_arcade_led();
  fill_board_solid(CRGB::Black);

  for (int i = 0; i < max_correct; i++)
  {
    if (game_state == NUMBER_WAND_STATE)
    {
      random_game_pieces_list[i] = get_unique_random_gamepiece(NUM_NUMBERS, game_pieces.numbers, random_game_pieces_list, max_correct);
      fill_numbers_solid(CRGB::Yellow);
    }
    else
    {
      random_game_pieces_list[i] = get_unique_random_gamepiece(NUM_LETTERS, game_pieces.letters, random_game_pieces_list, max_correct);
      fill_letters_solid(CRGB::Yellow);
    }
  }

  Serial.println("Random game pieces list:");
  for (int i = 0; i<max_correct; i++){
    print_single_game_piece(random_game_pieces_list[i]);
  }
  Serial.println("");

  while (correct_selections < max_correct)
  {
    GamePiece current_game_piece = random_game_pieces_list[correct_selections];

    Serial.print("Find the following tile: ");
    Serial.println((char)current_game_piece.character);

    if( !hint_is_active || repeat == 1) {
      send_serial_audio_command(current_game_piece);
      repeat = 0;
    }
    uint8_t uidLength;
    uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
    while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30) && !utility_button_pressed)
    {
      delay(50); // avoid overwhelming the RFID reader
    }

    if (utility_button_pressed)
    {
      switch (utility_button_pressed){
        case HINT_BUTTON_PIN:
          if (!hint_is_active){

            hint_is_active = 1;

            Serial.println("Hint button pressed. Exiting game.");
            if (game_state == NUMBER_WAND_STATE){
              for (int i = 0; i < max_hints; i++)
              {
                hint_game_pieces_list[i] = get_unique_random_gamepiece(NUM_NUMBERS, game_pieces.numbers, random_game_pieces_list, max_correct);
                print_single_game_piece(hint_game_pieces_list[i]);
              }
            }

            else{
              Serial.println("Hint button pressed ELSE");
              for (int i = 0; i < max_hints; i++)
              {
                hint_game_pieces_list[i] = get_unique_random_gamepiece(NUM_LETTERS, game_pieces.letters, random_game_pieces_list, max_correct);
                print_single_game_piece(hint_game_pieces_list[i]);
              }
            }

            hint_game_pieces_list[max_hints] = current_game_piece;
            print_single_game_piece(hint_game_pieces_list[max_hints]);
            for (int i = 0; i < max_hints + 1; i++){
              illuminate_single_game_piece(hint_game_pieces_list[i], CRGB(250, 70, 0));
            }
          }

        break;

      case END_GAME_BUTTON_PIN:
        Serial.println("End game button pressed. Exiting game.");
        return;

      case REPEAT_BUTTON_PIN:
        Serial.println("Repeat button pressed. Repeating audio");
        repeat = 1;
        break;

        case SKIP_BUTTON_PIN:
          // Serial.println("Skip button pressed. Exiting game.");
          skip = 1;
          Serial.println("Skip button pressed. Exiting game.");
          Serial.println(digitalRead(SKIP_BUTTON_PIN));
          break;
      }
      utility_button_pressed = 0; // Reset the button press
    }

    if (uids_match(uid, current_game_piece.uid) || skip)
    {
      if (hint_is_active){
        for (int i = 0; i < max_hints+1; i++){
          illuminate_single_game_piece(hint_game_pieces_list[i], CRGB::Yellow);
        }
        hint_is_active = 0;
      }

      flash_tile_location(current_game_piece, CRGB::Green, 2);
      illuminate_single_game_piece(current_game_piece, CRGB::Green);
      correct_selections++;
      skip = 0; // Reset skip flag
    }

    else if (uid_is_uid_of_a_previous_gamepiece_in_list(correct_selections, random_game_pieces_list, uid) || repeat)
    {
      continue; // Don't flash the tile if it's already green
    }

    else if ((game_state == NUMBER_WAND_STATE) && uid_is_uid_of_a_previous_gamepiece_in_list(NUM_LETTERS, game_pieces.letters, uid))
    {
      continue; // Don't flash the tile if it's from the letters during number game
    }

    else if ((game_state == LETTER_WAND_STATE || game_state == ENUNCIATION_STATE) && uid_is_uid_of_a_previous_gamepiece_in_list(NUM_NUMBERS, game_pieces.numbers, uid))
    {
      continue; // Don't flash the tile if it's from the numbers during letter game
    }

    else
    {
      GamePiece incorrect_gamepiece = get_gamepiece_by_uid(uid);
      flash_tile_location(incorrect_gamepiece, CRGB::Red, 2);
      illuminate_single_game_piece(incorrect_gamepiece, CRGB::Yellow);
      Serial.println("Incorrect selection. Try again!");
    }
  }
  game_state = GAME_OVER_STATE;
  Serial.println("Game complete: 5 correct selections achieved!");
}
