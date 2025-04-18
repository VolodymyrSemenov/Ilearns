// Game state
extern bool gameWon;

void begin_game_letter_ordering() {
    Serial.println("Starting Letter Ordering Game...");
    
    // Turn off button LEDs of other games
    digitalWrite(numberOrderingLED, LOW);
    digitalWrite(findLettersLED, LOW);
    digitalWrite(findNumbersLED, LOW);
    digitalWrite(letterOrderingLED, HIGH); // Turn on LED for this game
    
    char expectedLetters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                              'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 
                              'V', 'W', 'X', 'Y', 'Z'};
    
    bool gameComplete = false;
    while (!gameComplete) {
        // Check end game button
        if (digitalRead(endGameBtnPin) == LOW) {
            Serial.println("End Game button pressed. Exiting Letter Ordering Game.");
            return;
        }
        
        bool allCorrect = true; // Assume all are correct initially
        
        for (int i = 0; i < 26; i++) { // Loop through all 26 placements
            uint8_t uid[7];
            uint8_t uidLength;
            nfc.SAMConfig();
            bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
            
            if (success) {
                char detectedLetter = getLetterFromUID(uid, uidLength);
                if (detectedLetter != expectedLetters[i]) {
                    allCorrect = false;
                }
            } else {
                allCorrect = false;
            }
        }
        
        if (allCorrect) {
            gameComplete = true;
            Serial.println("Congratulations! Letters are in the correct order.");
            lightUpGreen();
        } else {
            Serial.println("Incorrect order. Keep trying!");
            lightUpRed();
            delay(1000);
        }
    }
    // Game state
    extern bool gameWon;
}
