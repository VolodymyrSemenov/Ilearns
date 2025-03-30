/**************************************************************************
 * Find Letters Annunciation Game
 * 
 * This game asks the user (via the wand input) to match a random letter by
 * presenting the correct RFID tag. For each round, the corresponding audio 
 * file for the prompted letter is played (using letter pointing audio).
 * The game continues until 5 letters are correctly matched, then exits.
 * 
 * This file assumes that the global 'letters' array (of type gamePiece),
 * the LED array 'letter_crgb_leds' (for FastLED), the NFC instance 'nfc', and
 * audio configuration functions (from audio_config.ino) are defined in the
 * main file.
 **************************************************************************/

// Include the audio configuration file.
#include "audio_config.ino"

// Wand RFID reading functions.
String uidToString(uint8_t *uid, uint8_t uidLength) {
  String tag = "";
  for (uint8_t i = 0; i < uidLength; i++) {
    if (uid[i] < 0x10) tag += "0";
    tag += String(uid[i], HEX);
  }
  tag.toLowerCase();
  return tag;
}

String readWandTag() {
  uint8_t uid[7];
  uint8_t uidLength = 0;
  bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 5000);
  if (success && uidLength > 0) {
    String tag = uidToString(uid, uidLength);
    Serial.print("Wand read tag: ");
    Serial.println(tag);
    return tag;
  }
  Serial.println("No tag read from wand.");
  return "";
}

// Returns a random letter from the global letters array.
String getRandomLetter() {
  int index = random(0, num_letters);
  return String(letters[index].character);
}

// Checks if the RFID tag read from the wand matches the expected tag
// for the given letter. The expected tag is simulated as "tag" + (index+1).
bool checkRFIDTagMatch(String currentLetter, String readTag) {
  for (int i = 0; i < num_letters; i++) {
    if (String(letters[i].character) == currentLetter) {
      String expectedTag = "tag" + String(i + 1);
      Serial.print("Expected tag: ");
      Serial.println(expectedTag);
      return (expectedTag == readTag);
    }
  }
  return false;
}

// Main game loop: prompts the user for a random letter, plays its audio file,
// reads the wand's RFID tag, checks for a match, and repeats until 5 correct matches.
void begin_game_find_letters_annunciation() {
  int correctSelections = 0;
  const int maxCorrect = 5;
  Serial.println("Starting Find Letters Annunciation Game...");
  
  while (correctSelections < maxCorrect) {
    String randomLetter = getRandomLetter();
    Serial.print("Find the letter: ");
    Serial.println(randomLetter);
    
    // Play the audio file for the selected letter (using gameState 2: Letter Pointing).
    playLetterAudio(randomLetter, 2);
    
    // Wait for the wand to present a tag.
    String tagBeingRead = readWandTag();
    if (tagBeingRead == "") {
      delay(500);
      continue;
    }
    
    if (checkRFIDTagMatch(randomLetter, tagBeingRead)) {
      Serial.println("Correct match!");
      correctSelections++;
      fill_solid(letter_crgb_leds, num_letter_leds, CRGB::Green);
      FastLED.show();
    } else {
      Serial.println("Incorrect match, try again.");
      fill_solid(letter_crgb_leds, num_letter_leds, CRGB::Red);
      FastLED.show();
    }
    
    delay(1500);
    fill_solid(letter_crgb_leds, num_letter_leds, CRGB::Black);
    FastLED.show();
  }
  
  Serial.println("Game complete: 5 correct selections achieved!");
}
