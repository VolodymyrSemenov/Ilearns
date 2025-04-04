// Game state
extern bool gameWon;

void begin_game_number_ordering() {
    Serial.println("Number Ordering Game Started");
    
    // Turn off LEDs of other games
    digitalWrite(letterOrderingLED, LOW);
    digitalWrite(findLettersLED, LOW);
    digitalWrite(findNumbersLED, LOW);
    
    // Turn on number ordering LED
    digitalWrite(numberOrderingLED, HIGH);
    
    bool game_complete = false;
    while (!game_complete) {
        if (digitalRead(endGameBtnPin) == LOW) {
            Serial.println("End Game button pressed. Exiting Number Ordering Game.");
            return;
        }
        
        bool all_correct = true;
        
        for (int i = 0; i <= 20; i++) {  // Checking positions 0-20
            uint8_t uid[7];
            uint8_t uidLength;
            
            nfc.setCurrentReader(readers[i]);
            nfc.SAMConfig();
            bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
            
            if (success) {
                Serial.print("Reader "); Serial.print(i);
                Serial.print(" detected UID: ");
                for (uint8_t j = 0; j < uidLength; j++) {
                    Serial.print(" 0x"); Serial.print(uid[j], HEX);
                }
                Serial.println("");
                
                if (!is_correct_number(i, uid, uidLength)) {
                    all_correct = false;
                    illuminate_leds(CRGB::Red, i);
                } else {
                    illuminate_leds(CRGB::Green, i);
                }
            } else {
                Serial.print("No tile detected at position "); Serial.println(i);
                all_correct = false;
                illuminate_leds(CRGB::Red, i);
            }
        }
        
        if (all_correct) {
            Serial.println("All numbers in correct order! You win!");
            lightUpGreen();
            game_complete = true;
        }
        
        delay(1000);
    }
    gameWon = true;
    Serial.println("Number Ordering Game Ended");
}
