/**************************************************************************
 * Find Numbers Game
 **************************************************************************/

 #include "audio_config.ino"

 // --- Declare End Game Button Pin (defined in the main file) ---
 extern const int endGameBtnPin;
 
 // --- Decoder Selection Function ---
 const int DECODER_PINS[6] = {10, 11, 12, 13, 14, 15}; // Replace with actual pins later
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
   selectRFIDReader(0);  // Always use the wand reader (reader 0)
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
 String getRandomNumber() {
   int index = random(0, num_numbers);
   return String(numbers[index].character);
 }
  
 bool checkRFIDTagMatch_Number(String currentNumber, String readTag) {
   for (int i = 0; i < num_numbers; i++) {
     if (String(numbers[i].character) == currentNumber) {
       String expectedTag = "tag" + String(i + 1);
       Serial.print("Expected tag: ");
       Serial.println(expectedTag);
       return (expectedTag == readTag);
     }
   }
   return false;
 }
  
 void begin_game_find_numbers() {
   int correctSelections = 0;
   const int maxCorrect = 5;
   Serial.println("Starting Find Numbers Game...");
    
   while (correctSelections < maxCorrect) {
     // Select a target number for this round
     String randomNumber = getRandomNumber();
     Serial.print("Find the number: ");
     Serial.println(randomNumber);
      
     // Play the audio file for the selected number (gameState 1: Number Pointing)
     playLetterAudio(randomNumber, 1);
      
     // Wait until a tile is scanned
     String tagBeingRead;
     while (true) {
       if (digitalRead(endGameBtnPin) == LOW) {
         Serial.println("End Game button pressed. Exiting Find Numbers Game.");
         return;
       }
       
       tagBeingRead = readWandTag();
       if (tagBeingRead == "") {
         delay(500);
         continue;
       }
       
       if (checkRFIDTagMatch_Number(randomNumber, tagBeingRead)) {
         Serial.println("Correct match!");
         correctSelections++;
         fill_solid(number_crgb_leds, num_number_leds, CRGB::Green);
         FastLED.show();
         break;  // Correct tile found, exit waiting loop
       } else {
         Serial.println("Incorrect match, try again.");
         fill_solid(number_crgb_leds, num_number_leds, CRGB::Red);
         FastLED.show();
       }
       
       delay(1500);
       fill_solid(number_crgb_leds, num_number_leds, CRGB::Black);
       FastLED.show();
     }
   }
    
   Serial.println("Game complete: 5 correct selections achieved!");
 }
 