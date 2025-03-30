


/**************************************************************************/
/*!
    @file     iso14443a_uid.pde
    @author   Adafruit Industries
	@license  BSD (see license.txt)

    This example will attempt to connect to an ISO14443A
    card or tag and retrieve some basic information about it
    that can be used to determine what type of card it is.

    Note that you need the baud rate to be 115200 because we need to print
	out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout
  ----> https://www.adafruit.com/products/364

Check out the links above for our tutorials and wiring diagrams
These chips use SPI or I2C to communicate.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <FastLED.h>

#define LED_PIN     10   // Pin where the data line is connected
#define NUM_LEDS    6 // Number of LEDs in the strand
#define BRIGHTNESS  128 // Half brightness (max is 255)
#define WIDTH_PER_PIECE 1

CRGB leds[NUM_LEDS];

// If using the breakout with SPI, define the pins for SPI communication.
// #define PN532_SCK  (2)
// #define PN532_MOSI (3)
// #define PN532_SS   (4)
// #define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
// #define PN532_IRQ   (2)
// #define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

uint8_t decoderPins[] = {30, 31, 32, 33, 34, 35, 36}; // EN through A5
// Use this line for a breakout with a SPI connection:
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
 Adafruit_PN532 nfc(decoderPins, &SPI, 10);
  uint8_t readers[] = {1};
  uint8_t card_not_present[sizeof(readers)];
// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// Forward declaration of functions
void illuminate_leds(CRGB color, int positions);
void lightUpWhite();
void lightUpRed();
void lightUpGreen();
void lightUpBlue();
void lightUpYellow();
void lightUpPurple();

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10); // for Leonardo/Micro/Zero
  Serial.println("Hello!");
  nfc.begin();



  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  digitalWrite(11, HIGH);
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  lightUpWhite();
  // SPI.setClockDivider(SPI_CLOCK_DIV8);

  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
  uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
   for(int i = 0; i < sizeof(readers); i++) {
    delay(50);
    nfc.begin(); // Calls nfc.wakeup that sets SAMConfig
    // nffc.wakeup();
    // nfc.SAMConfig();
    nfc.setCurrentReader(readers[i]);


    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50);

    // for(int j = 0; j < 3; j++){

      // Serial.print("j for loop with j="); Serial.println(j);

    if (success) {
      Serial.print("Reader on decoder line "); Serial.println(readers[i]);
      Serial.println("Found a card!");
      Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t i=0; i < uidLength; i++)
      {
        Serial.print(" 0x");Serial.print(uid[i], HEX);
      }
      Serial.println("");
      illuminate_leds(CRGB::Green, i);
      card_not_present[i] = 0;
      // lightUpWhite();
      // delay(1000);
      // lightUpRed();
      // delay(1000);
      // lightUpGreen();
      // delay(1000);
      // lightUpBlue();
      // delay(1000);
      // j = 3;
      // }
      // success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50);
    }

    else{
      int failed_attempts = card_not_present[i];

      if (failed_attempts < 2) {
        card_not_present[i] = failed_attempts + 1;
      }
      else{
        card_not_present[i] = 2;
        illuminate_leds(CRGB::Red, i);
      }
      // illuminate_leds(CRGB::Red,i);
    }
    
    // leds[0] = CRGB::Green;
    // FastLED.show();
  

 // }

Serial.println("end");
  }
}

// LED functions (same as before)
void illuminate_leds(CRGB color, int positions){
  leds[0] = CRGB::Black;
  for (int i=0; i<1; i++){
    leds[positions+1] = color;
  }
  FastLED.show();
}

void lightUpWhite() { fill_solid(leds, NUM_LEDS, CRGB(255, 255, 255)); FastLED.show(); }
void lightUpRed()   { fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0)); FastLED.show(); }
void lightUpGreen() { fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0)); FastLED.show(); }
void lightUpBlue()  { fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255)); FastLED.show(); }
void lightUpYellow(){ fill_solid(leds, NUM_LEDS, CRGB(255, 255, 0)); FastLED.show(); }
void lightUpPurple(){ fill_solid(leds, NUM_LEDS, CRGB(255, 0, 255)); FastLED.show(); }
// void lightUpOrange(){ fill_solid(leds, NUM_LEDS, orange_1); FastLED.show(); }
// void lightUpBla