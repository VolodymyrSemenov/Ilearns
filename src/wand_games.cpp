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
#include <illumination.h>
#include <printing.h>

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
GamePiece get_unique_random_gamepiece(int max_size, GamePiece game_piece_list[], GamePiece random_game_pieces_list[], int max_correct)
{
  int random_index = random(0, max_size);

  while (game_piece_is_in_list(game_piece_list[random_index], random_game_pieces_list, max_correct))
  {
    random_index = random(0, max_size);
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
  String game_piece_character_string;

  if (game_state == 4)
  {
    // Convert number as bytes to String
    game_piece_character_string = String((char)game_piece.character);
  }
  else
  {
    game_piece_character_string = String(game_piece.character);
  }
  Serial.println(game_piece_character_string);

  // Play audio to giga over serial
  // Serial1.print(",");      // Separator
  Serial1.print(game_state);                    // Send game state
  Serial1.println(game_piece_character_string); // Send character first
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

bool uid_is_uid_of_previous_gamepiece(int correct_selections, GamePiece random_game_pieces_list[], uint8_t uid[])
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

  constexpr int max_hints = 3; // plus the correct one
  GamePiece hint_game_pieces_list[max_hints + 1];
  int hint = 0;
  int repeat = 0;
  int skip = 0;
  generate_random_seed();

  illuminate_active_game_arcade_led();

  for (int i = 0; i < max_correct; i++)
  {
    fill_board_solid(CRGB::Black);
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

  while (correct_selections < max_correct)
  {
    GamePiece current_game_piece = random_game_pieces_list[correct_selections];

    Serial.print("Find the following tile: ");
    Serial.println(current_game_piece.character);

    send_serial_audio_command(current_game_piece);

    uint8_t uidLength;
    uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};
    while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30) && !utility_button_pressed)
    {
      delay(50); // avoid overwhelming the RFID reader
    }

    if (utility_button_pressed)
    {
      switch (utility_button_pressed)
      {
      case HINT_BUTTON_PIN:
        if (!hint)
        {

          Serial.println("Hint button pressed. Exiting game.");
          if (game_state == NUMBER_WAND_STATE)
          {
            for (int i = 0; i < max_hints; i++)
            {
              hint_game_pieces_list[i] = get_unique_random_gamepiece(NUM_NUMBERS, game_pieces.numbers, hint_game_pieces_list, max_hints);
            }
          }

          else
          {
            for (int i = 0; i < max_hints; i++)
            {
              hint_game_pieces_list[i] = get_unique_random_gamepiece(NUM_LETTERS, game_pieces.letters, hint_game_pieces_list, max_hints);
            }
          }

          // for (int i=0; i<sizeof(hint_game_pieces_list); i++){
          //   print_single_game_piece(hint_game_pieces_list[i]);
          // }

          hint_game_pieces_list[max_hints] = current_game_piece;
          for (int i = 0; i < max_hints + 1; i++)
          {
            illuminate_single_game_piece(hint_game_pieces_list[i], CRGB(250, 70, 0));
          }
          hint = 1;
        }

        break;

      case END_GAME_BUTTON_PIN:
        Serial.println("End game button pressed. Exiting game.");
        correct_selections = max_correct; // End the game
        break;

      case REPEAT_BUTTON_PIN:
        Serial.println("Repeat button pressed. Repeating audio");
        send_serial_audio_command(current_game_piece);
        repeat = 1;
        break;

      case SKIP_BUTTON_PIN:
        skip = 1;
        Serial.println("Skip button pressed. Exiting game.");
        break;
      }
      utility_button_pressed = 0; // Reset the button press
    }

    if (uids_match(uid, current_game_piece.uid) || skip)
    {
      if (hint)
      {
        for (int i = 0; i < max_hints + 1; i++)
        {
          illuminate_single_game_piece(hint_game_pieces_list[i], CRGB::Yellow);
        }
        hint = 0;
      }

      flash_tile_location(current_game_piece, CRGB::Green, 2);
      illuminate_single_game_piece(current_game_piece, CRGB::Green);
      correct_selections++;
      skip = 0; // Reset skip flag
    }

    else if (uid_is_uid_of_previous_gamepiece(correct_selections, random_game_pieces_list, uid) || repeat)
    {
      repeat = 0;
      continue; // Don't flash the tile if it's already green
    }
    else
    {
      GamePiece incorrect_gamepiece = get_gamepiece_by_uid(uid);
      flash_tile_location(incorrect_gamepiece, CRGB::Red, 2);
      illuminate_single_game_piece(incorrect_gamepiece, CRGB::Yellow);
      Serial.println("Incorrect selection. Try again!");
    }
  }
  Serial.println("Game complete: 5 correct selections achieved!");
}
