// #include <Arduino.h>

// // (c) Michael Schoeffler 2014, http://www.mschoeffler.de

// #include "SPI.h" // SPI library
// #include "MFRC522.h" // RFID library (https://github.com/miguelbalboa/rfid)

// const int pinRST = 9;
// const int pinSDA = 10;

// MFRC522 mfrc522(pinRST); // Set up mfrc522 on the Arduino

// void selectMuxChannel(int channel) {
//     int active = HIGH;
//   switch (channel) {
//     case 0:
//       digitalWrite(6, active);
//       digitalWrite(8, !active);
//       digitalWrite(10, !active);
//       break;
//     case 1:
//       digitalWrite(6, !active);
//       digitalWrite(8, active);
//       digitalWrite(10, !active);
//       break;
//     case 2:
//       digitalWrite(6, !active);
//       digitalWrite(8, !active);
//       digitalWrite(10, active);
//       break;
//   }
// }

// void setup() {
//   SPI.begin(); // open SPI connectiona
//   selectMuxChannel(2);
//   delay(100);
//   mfrc522.PCD_Init(); // Initialize Proximity Coupling Device (PCD)
//   Serial.begin(9600); // open serial connection
// }

// void loop() {
//   selectMuxChannel(2);
//   if (mfrc522.PICC_IsNewCardPresent()) { // (true, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
//     if(mfrc522.PICC_ReadCardSerial()) { // true, if RFID tag/card was read
//       Serial.print("RFID TAG ID:");
//       for (byte i = 0; i < mfrc522.uid.size; ++i) { // read id (in parts)
//         Serial.print(mfrc522.uid.uidByte[i], HEX); // print id as hex values
//         Serial.print(" "); // add space between hex blocks to increase readability
//       }
//       Serial.println(); // Print out of id is complete.
//     }
//   }
// }