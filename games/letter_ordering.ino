void begin_game_letter_ordering() {
    Serial.println("Starting Letter Ordering Game...");
    
    // Turn off button LEDs of other games (assuming they are defined elsewhere)
    digitalWrite(numberOrderingLED, LOW);
    digitalWrite(findLettersLED, LOW);
    digitalWrite(findNumbersLED, LOW);
    digitalWrite(letterOrderingLED, HIGH); // Turn on LED for this game
    
    char expectedLetters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                              'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 
                              'V', 'W', 'X', 'Y', 'Z'}; // Expected order
    
    bool gameComplete = false;
    while (!gameComplete) {
        bool allCorrect = true; // Assume all are correct initially
        
        for (int i = 0; i < 26; i++) { // Loop through all 26 placements
            uint8_t uid[7]; // Buffer for UID
            uint8_t uidLength;
            bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
            
            if (success) {
                char detectedLetter = getLetterFromUID(uid, uidLength); // Convert UID to letter
                
                if (detectedLetter != expectedLetters[i]) {
                    allCorrect = false; // Found incorrect letter
                }
            } else {
                allCorrect = false; // No tag detected at this position
            }
        }
        
        if (allCorrect) {
            gameComplete = true;
            Serial.println("Congratulations! Letters are in the correct order.");
            lightUpGreen(); // Indicate success
        } else {
            Serial.println("Incorrect order. Keep trying!");
            lightUpRed(); // Indicate incorrect placement
            delay(1000); // Small delay before checking again
        }
    }
}

// Function to map a UID to a letter (mock function, replace with actual logic)
char getLetterFromUID(uint8_t *uid, uint8_t uidLength) {
    // Placeholder: Convert UID to a letter (this will depend on actual tag mappings)
    return (char) uid[0];
}
