// #include <Arduino.h>

// // (c) Michael Schoeffler 2014, http://www.mschoeffler.de

// #include "SPI.h" // SPI library
// #include "MFRC522.h" // RFID library (https://github.com/miguelbalboa/rfid)

// const int pinRST = 9;
// const int pinSDA = 10;

// byte ssPins[] = {8, 10};

// MFRC522 mfrc522[2];   // Create MFRC522 instance


// /**
//  * Helper routine to dump a byte array as hex values to Serial.
//  */
// void dump_byte_array(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], HEX);
//   }
// }



// void setup() {
//   SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
//   SPI.begin(); // open SPI connectiona
//   for (uint8_t reader = 0; reader < 2; reader++) {
//     mfrc522[reader].PCD_Init(ssPins[reader], 9); // Init each MFRC522 card
//     Serial.print(F("Reader "));
//     Serial.print(reader);
//     Serial.print(F(": "));
//     mfrc522[reader].PCD_DumpVersionToSerial();
//   }
//   Serial.begin(9600); // open serial connection
// }
// void loop() {

//   for (uint8_t reader = 0; reader < 2; reader++) {
//     // Look for new cards

//     if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
//       Serial.print(F("Reader "));
//       Serial.print(reader);
//       // Show some details of the PICC (that is: the tag/card)
//       Serial.print(F(": Card UID:"));
//       dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
//       Serial.println();
//       //Serial.print(F("PICC type: "));
//       //MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
//       //Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

//       // Halt PICC
//       mfrc522[reader].PICC_HaltA();
//       // Stop encryption on PCD
//       mfrc522[reader].PCD_StopCrypto1();

//     } //if (mfrc522[reader].PICC_IsNewC
//   } //for(uint8_t reader
// }
