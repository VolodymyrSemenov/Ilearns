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

// Declare external buttons (defined in the main file)
// extern const int END_GAME_BUTTON_PIN;
// extern const int SKIP_BUTTON_PIN;
// extern const int REPEAT_BUTTON_PIN;
// extern const int HINT_BUTTON_PIN;

extern int game_state;
extern bool game_over;

// extern CRGB letter_crgb_leds[];
// extern CRGB number_crgb_leds[];
// extern Adafruit_PN532 nfc;

// --- Decoder Selection Function ---
constexpr int DECODER_PINS[6] = {10, 11, 12, 13, 14, 15}; // Replace with actual pins later
void selectRFIDReader(int readerNumber) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(DECODER_PINS[i], (readerNumber >> i) & 1);
  }
  delay(10);
}

// --- Wand RFID Reading Functions ---
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
  selectRFIDReader(0);
  uint8_t uid[7];
  uint8_t uidLength = 0;
  nfc.SAMConfig();
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

// --- Game Functions ---
String getRandomNumber() {
  int index = random(0, NUM_NUMBERS);
  return String(game_pieces.numbers[index].character);
}

bool checkRFIDTagMatch_Number(String currentNumber, String readTag) {
  for (int i = 0; i < NUM_NUMBERS; i++) {
    if (String(game_pieces.numbers[i].character) == currentNumber) {
      String expectedTag = "tag" + String(i + 1);
      Serial.print("Expected tag: ");
      Serial.println(expectedTag);
      return (expectedTag == readTag);
    }
  }
  return false;
}

void begin_game_find_numbers() {
  game_over = false;

  while (!game_over){
    int correctSelections = 0;
    constexpr int maxCorrect = 5;
    String random_numbers_list[maxCorrect];
    Serial.println("Starting Find Numbers Game...");

    for (int i=0; i<maxCorrect; i++){
      random_numbers_list[i] = getRandomNumber();

    }
    
    while (correctSelections < maxCorrect) {
      // Select a target number for this round
      String randomNumber = getRandomNumber();
      Serial.print("Find the number: ");
      Serial.println(randomNumber);
      
      // Play audio prompt (gameState 1: Number Pointing)
      // playLetterAudio(randomNumber, 1);
      
      String tagBeingRead;
      while (true) {
        // Check buttons while waiting:
        if (digitalRead(END_GAME_BUTTON_PIN) == LOW) {
          Serial.println("End Game button pressed. Exiting Find Numbers Game.");
          return;
        }
        if (digitalRead(SKIP_BUTTON_PIN) == LOW) {
          Serial.println("Skip button pressed. Moving to next target.");
          break;
        }
        if (digitalRead(REPEAT_BUTTON_PIN) == LOW) {
          Serial.println("Repeat button pressed. Replaying audio.");
          // playLetterAudio(randomNumber, 1);
          delay(500);
        }
        
        tagBeingRead = readWandTag();
        if (tagBeingRead == "") {
          delay(500);
          continue;
        }
        
        if (checkRFIDTagMatch_Number(randomNumber, tagBeingRead)) {
          Serial.println("Correct match!");
          correctSelections++;
          fill_solid(number_crgb_leds, NUM_NUMBER_LEDS, CRGB::Green);
          FastLED.show();
          break;  // Exit waiting loop if correct
        } else {
          Serial.println("Incorrect match, try again.");
          fill_solid(number_crgb_leds, NUM_NUMBER_LEDS, CRGB::Red);
          FastLED.show();
        }
        
        delay(1500);
        fill_solid(number_crgb_leds, NUM_NUMBER_LEDS, CRGB::Black);
        FastLED.show();
      }
    }

    game_over = true;
    Serial.println("Game complete: 5 correct selections achieved!");
  }
}
