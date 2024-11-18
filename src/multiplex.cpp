// #include <SPI.h>
// #include <MFRC522.h>

// // Define the number of readers
// #define NUM_READERS 3

// // // Define Arduino pins for the multiplexer control
// // const int muxS0 = 2;
// // const int muxS1 = 3;

// // Define the reader reset pin (shared among readers)
// const int RST_PIN = 9;

// // Define an array of MFRC522 objects
// MFRC522 readers[NUM_READERS] = {MFRC522(2, RST_PIN), MFRC522(2, RST_PIN), MFRC522(2, RST_PIN)};

// // Function to select the multiplexer channel
// void selectMuxChannel(int channel) {
//   switch (channel) {
//     case 0:
//       digitalWrite(6, LOW);
//       digitalWrite(8, HIGH);
//       digitalWrite(10, HIGH);
//       break;
//     case 1:
//       digitalWrite(6, HIGH);
//       digitalWrite(8, LOW);
//       digitalWrite(10, HIGH);
//       break;
//     case 2:
//       digitalWrite(6, HIGH);
//       digitalWrite(8, HIGH);
//       digitalWrite(10, LOW);
//       break;
//   }
// }

// void setup() {
//   Serial.begin(9600);
//   SPI.begin();        // Initialize SPI bus

//   // Set multiplexer control pins as outputs
//   pinMode(6, OUTPUT);
//   pinMode(8, OUTPUT);
//   pinMode(10, OUTPUT);
//   pinMode(9, OUTPUT);

//   // Initialize each reader
//   for (int i = 0; i < NUM_READERS; i++) {
//     selectMuxChannel(i);
//     readers[i].PCD_Init(); // Initialize the MFRC522 reader
//     delay(50);             // Small delay to ensure proper initialization
//   }
//   digitalWrite(9, HIGH);
// }

// void loop() {
//   for (int i = 0; i < NUM_READERS; i++) {
//     // Select the multiplexer channel for this reader
//     selectMuxChannel(i);

//     // Check if a card is present
//     if (readers[i].PICC_IsNewCardPresent() && readers[i].PICC_ReadCardSerial()) {
//       Serial.print("Reader ");
//       Serial.print(i);
//       Serial.print(": Card UID:");

//       // Print UID
//       for (byte j = 0; j < readers[i].uid.size; j++) {
//         Serial.print(readers[i].uid.uidByte[j] < 0x10 ? " 0" : " ");
//         Serial.print(readers[i].uid.uidByte[j], HEX);
//       }
//       Serial.println();

//     }
//   }

//   delay(500); // Adjust delay as needed
// }
