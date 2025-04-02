/**************************************************************************
 * Find Letters Annunciation Game
 **************************************************************************/

 #include "audio_config.ino"

 // Declare external buttons (defined in the main file)
 extern const int endGameBtnPin;
 extern const int skipBtnPin;
 extern const int repeatBtnPin;
 extern const int extraBtnPin;
 
 // --- Decoder Selection Function ---
 const int DECODER_PINS[6] = {10, 11, 12, 13, 14, 15}; // Replace with actual pins later
 void selectRFIDReader(int readerNumber) {
   for (int i = 0; i < 6; i++) {
     digitalWrite(DECODER_PINS[i], (readerNumber >> i) & 1);
   }
   delay(10); // Allow time for the decoder to settle.
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
 String getRandomLetter() {
   int index = random(0, num_letters);
   return String(letters[index].character);
 }
  
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
  
 void begin_game_find_letters_annunciation() {
   int correctSelections = 0;
   const int maxCorrect = 5;
   Serial.println("Starting Find Letters Annunciation Game...");
    
   while (correctSelections < maxCorrect) {
     // Select a target letter for this round
     String randomLetter = getRandomLetter();
     Serial.print("Find the letter: ");
     Serial.println(randomLetter);
      
     // Play audio prompt (gameState 2: Letter Pointing)
     playLetterAudio(randomLetter, 2);
     
     String tagBeingRead;
     while (true) {
       // Check buttons while waiting for a scan:
       if (digitalRead(endGameBtnPin) == LOW) {
         Serial.println("End Game button pressed. Exiting Find Letters Annunciation Game.");
         return;
       }
       if (digitalRead(skipBtnPin) == LOW) {
         Serial.println("Skip button pressed. Moving to next target.");
         break; // Abandon this target and pick a new one
       }
       if (digitalRead(repeatBtnPin) == LOW) {
         Serial.println("Repeat button pressed. Replaying audio.");
         playLetterAudio(randomLetter, 2);
         delay(500); // Debounce delay
       }
       
       tagBeingRead = readWandTag();
       if (tagBeingRead == "") {
         delay(500);
         continue;
       }
       
       if (checkRFIDTagMatch(randomLetter, tagBeingRead)) {
         Serial.println("Correct match!");
         correctSelections++;
         fill_solid(letter_crgb_leds, num_letter_leds, CRGB::Green);
         FastLED.show();
         break;  // Correct tile found, exit waiting loop
       } else {
         Serial.println("Incorrect match, try again.");
         fill_solid(letter_crgb_leds, num_letter_leds, CRGB::Red);
         FastLED.show();
       }
       
       delay(1500);
       fill_solid(letter_crgb_leds, num_letter_leds, CRGB::Black);
       FastLED.show();
     }
   }
    
   Serial.println("Game complete: 5 correct selections achieved!");
 }
 