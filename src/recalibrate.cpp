#include <recalibrate.h>

#include <EEPROM.h>
#include <structures.h>
#include <illumination.h>
#include <printing.h>
#include <games.h>



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
    int delay_time_between_reads = 1000;
    bool uidAccepted;
    bool uidRead;

    // Letters
    for (int i = 0; i < NUM_LETTERS; i++)
    {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs
        illuminate_next_letter_tile_location(i, CRGB::Yellow);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30) && !utility_button_pressed)
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        uidAccepted = false;
        uidRead = false;

        // Reads uid until a unique one is found or the utility button is pressed
        while (!uidAccepted && !utility_button_pressed) {
            if (!uidRead && nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30)) {
                uidRead = true;

                if (uid_is_uid_of_a_previous_gamepiece_in_list(i, game_pieces.letters, uid)) {
                    flash_next_letter_tile_location(i, CRGB::Red, 2);
                    illuminate_next_letter_tile_location(i, CRGB::Yellow);
                    uidRead = false; // wait for a new read
                } else {
                    uidAccepted = true;
                }
            }
        }


        if (utility_button_pressed){
            if (utility_button_pressed == END_GAME_BUTTON_PIN){
                utility_button_pressed = 0;
                return;  
            }
            utility_button_pressed = 0;
        }

        game_pieces.letters[i] = generate_single_game_piece(game_pieces.letters[i], lower_case_ascii_offset + i, uid, i, i);
        illuminate_single_game_piece(game_pieces.letters[i], CRGB::Green);

        print_single_game_piece(game_pieces.letters[i]);
        delay(delay_time_between_reads);
    }

    // Numbers
    for (int i = 0; i < NUM_NUMBERS; i++)
    {
        uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0}; // Supports both 4-byte and 7-byte UIDs

        illuminate_next_number_tile_location(i, CRGB::Yellow);

        while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30) && !utility_button_pressed)
        {
            delay(50); // avoid overwhelming the RFID reader
        }

        uidAccepted = false;
        uidRead = false;
        // Reads uid until a unique one is found or the utility button is pressed
        while (!uidAccepted && !utility_button_pressed) {
            if (!uidRead && nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30)) {
                uidRead = true;

                if (uid_is_uid_of_a_previous_gamepiece_in_list(NUM_LETTERS, game_pieces.letters, uid)) {
                    flash_next_number_tile_location(i, CRGB::Red, 2);
                    illuminate_next_number_tile_location(i, CRGB::Yellow);
                    uidRead = false; // wait for a new read
                } 
                else if (uid_is_uid_of_a_previous_gamepiece_in_list(i, game_pieces.numbers, uid)) {
                    flash_next_number_tile_location(i, CRGB::Red, 2);
                    illuminate_next_number_tile_location(i, CRGB::Yellow);
                    uidRead = false; // wait for a new read
                } else {
                    uidAccepted = true;
                }
            }
        }

        if (utility_button_pressed){
            if (utility_button_pressed == END_GAME_BUTTON_PIN){
                utility_button_pressed = 0;
                return;  
            }
            utility_button_pressed = 0;
        }

        game_pieces.numbers[i] = generate_single_game_piece(game_pieces.numbers[i], i, uid, i + NUM_LETTERS, i);
        illuminate_single_game_piece(game_pieces.numbers[i], CRGB::Green);

        print_single_game_piece(game_pieces.numbers[i]);
        delay(delay_time_between_reads);
    }
    put_pieces_to_eeprom();
}

// Recalibrate game pieces and write to EEPROM
void recalibrate_game_pieces()
{
    fill_board_solid(CRGB::Black);
    generate_game_pieces_structure();
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