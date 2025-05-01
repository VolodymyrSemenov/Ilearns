#include <recalibrate.h>

#include <EEPROM.h>
#include <structures.h>
#include <illumination.h>
#include <printing.h>

// Given a base index, fill the positions array for a game piece.
void generate_positions(int baseIndex, int positions[])
{
    for (int i = 0; i < WIDTH_PER_PIECE; i++)
    {
        positions[i] = baseIndex + i;
    }
}

// Populate a single GamePiece struct
GamePiece generate_single_game_piece(GamePiece game_piece, byte character_value, uint8_t uid[], int decoder_value, int i)
{
    game_piece.character = character_value;
    memcpy(game_piece.uid, uid, MAX_UID_LENGTH);
    game_piece.decoder_value = decoder_value;
    generate_positions(OFFSET_BLANK_WS2811_LEDS + i * WIDTH_PER_PIECE, game_piece.positions);

    return game_piece;
}

// Generate the gamePiece structures for letters (A-Z) and numbers (0-20)
void generate_game_pieces_structure()
{
    int lower_case_ascii_offset = 97; // ASCII value for 'a'. Could also directly use 'a'
    uint8_t uidLength;

    // Letters
    for (int i = 0; i < NUM_LETTERS; i++)
    {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs
        Serial.println(i);
        Serial.println(OFFSET_BLANK_WS2811_LEDS);
        illuminate_next_letter_tile_location(i, CRGB::Yellow);

        Serial.println("size of letter_crgb_leds");
        Serial.println(sizeof(letter_crgb_leds) / 3);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        game_pieces.letters[i] = generate_single_game_piece(game_pieces.letters[i], lower_case_ascii_offset + i, uid, i, i);
        illuminate_single_game_piece(game_pieces.letters[i], CRGB::Green);

        print_single_game_piece(game_pieces.letters[i]);
        // delay(500);
    }

    // Numbers
    for (int i = 0; i < NUM_NUMBERS; i++)
    {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs

        illuminate_next_number_tile_location(i, CRGB::Yellow);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30))
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        game_pieces.numbers[i] = generate_single_game_piece(game_pieces.numbers[i], i, uid, i + NUM_LETTERS, i);
        illuminate_single_game_piece(game_pieces.numbers[i], CRGB::Green);

        print_single_game_piece(game_pieces.numbers[i]);
        // delay(500);
    }
}

// Write entire GamePieces struct to EEPROM (at game_pieces_address)
void put_pieces_to_eeprom()
{
    EEPROM.put(EEPROM_VALID_BIT_ADDRESS, 1);
    EEPROM.put(EEPROM_GAME_PIECES_ADDRESS, game_pieces);
}

// Read the GamePieces struct from EEPROM
GamePieces get_pieces_from_eeprom()
{
    GamePieces gp;
    EEPROM.get(EEPROM_GAME_PIECES_ADDRESS, gp);
    return gp;
}

// Recalibrate game pieces and write to EEPROM
void recalibrate_game_pieces()
{
    fill_board_solid(CRGB::Black);
    generate_game_pieces_structure();
    put_pieces_to_eeprom();
}

// Populate GamePieces struct
void populate_game_pieces_structure()
{
    int valid_bit = EEPROM.read(EEPROM_VALID_BIT_ADDRESS);
    if (valid_bit == 1)
    {
        game_pieces = get_pieces_from_eeprom();
    }
    else
    {
        recalibrate_game_pieces();
    }
}