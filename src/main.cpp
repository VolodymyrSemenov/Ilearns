


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

#define LED_PIN     41   // Pin where the data line is connected
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
 Adafruit_PN532 nfc(decoderPins, 4000000, &SPI, 30);





uint8_t readers[] = {14,24};
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
void printSPISpeed();

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10); // for Leonardo/Micro/Zero
  Serial.println("Hello!");
  // SPI.begin();
  // SPI.beginTransaction(SPISettings(125000, MSBFIRST, SPI_MODE0));

  nfc.begin(); // Calls nfc.wakeup that sets SAMConfig

  
  nfc.wakeup();
  nfc.SAMConfig();



  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  lightUpWhite();

  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};	// Buffer to store the returned UID
  uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  // printSPISpeed();
  for(size_t i = 0; i < sizeof(readers); i++) {

    // SPI.begin();
    // SPI.beginTransaction(SPISettings(125000, MSBFIRST, SPI_MODE0));
    // delay(250);
    nfc.setCurrentReader(readers[i]);
    // nfc.begin(); // Calls nfc.wakeup that sets SAMConfig
    // nfc.setPassiveActivationRetries(0xFF);
    // nfc.SAMConfig();

    // Check if the PN532 is connected
  // uint32_t versiondata = nfc.getFirmwareVersion();
  // if (!versiondata) {
  //   Serial.println("ERROR: No PN532 detected. Check your wiring!");
  //   while (1) { // Halt execution (optional)
  //     delay(1000);
  //   }
  // }

  // Serial.print("PN532 detected. Firmware version: 0x");
  // Serial.println(versiondata, HEX);

  // uint32_t versiondata = nfc.getFirmwareVersion();
  // if (versiondata) {
  //   Serial.print("PN532 detected. Firmware version: 0x");
  //   Serial.println(versiondata, HEX);
  // }


    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50);

    // for(int j = 0; j < 3; j++){

      // Serial.print("j for loop with j="); Serial.println(j);

    if (success) {
      // Serial.print("Reader on decoder line "); 
      Serial.println(readers[i]);
      Serial.println("Found a card!");
      Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t i=0; i < uidLength; i++)
      {
        Serial.print(" 0x");Serial.print(uid[i], HEX);
      }
      Serial.println("");

      illuminate_leds(CRGB::Green, i*WIDTH_PER_PIECE);
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
      int max_failed_attempts = 0;

      if (failed_attempts < max_failed_attempts) {
        card_not_present[i] = failed_attempts + 1;
      }
      else{
        card_not_present[i] = max_failed_attempts;
        illuminate_leds(CRGB::Red, i*WIDTH_PER_PIECE);
      }
    }

// Serial.println("end");
  // SPI.endTransaction();
  }
}

// LED functions (same as before)
void illuminate_leds(CRGB color, int positions){
  //leds[0] = CRGB::Black;
  int offset = 0 *WIDTH_PER_PIECE;
  for (int i=0; i<WIDTH_PER_PIECE-1; i++){
    leds[i+positions+offset] = color;
  }
  FastLED.show();
}

void lightUpWhite() { fill_solid(leds, NUM_LEDS, CRGB(255, 255, 255)); FastLED.show(); }
void lightUpRed()   { fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0)); FastLED.show(); }
void lightUpGreen() { fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0)); FastLED.show(); }
void lightUpBlue()  { fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255)); FastLED.show(); }
void lightUpYellow(){ fill_solid(leds, NUM_LEDS, CRGB(255, 255, 0)); FastLED.show(); }
void lightUpPurple(){ fill_solid(leds, NUM_LEDS, CRGB(255, 0, 255)); FastLED.show(); }
void lightUpBlack(){ fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0)); FastLED.show(); }
// void lightUpOrange(){ fill_solid(leds, NUM_LEDS, orange_1); FastLED.show(); }
// void lightUpBla

void printSPISpeed() {
  uint8_t spr1 = (SPCR & (1 << SPR1)) >> SPR1;
  uint8_t spr0 = (SPCR & (1 << SPR0)) >> SPR0;
  uint8_t spi2x = (SPSR & (1 << SPI2X)) >> SPI2X;

  uint16_t divider = 0;

  if (spr1 == 0 && spr0 == 0 && spi2x == 0) divider = 4;
  if (spr1 == 0 && spr0 == 0 && spi2x == 1) divider = 2;
  if (spr1 == 0 && spr0 == 1 && spi2x == 0) divider = 16;
  if (spr1 == 0 && spr0 == 1 && spi2x == 1) divider = 8;
  if (spr1 == 1 && spr0 == 0 && spi2x == 0) divider = 64;
  if (spr1 == 1 && spr0 == 0 && spi2x == 1) divider = 32;
  if (spr1 == 1 && spr0 == 1 && spi2x == 0) divider = 128;
  if (spr1 == 1 && spr0 == 1 && spi2x == 1) divider = 64;

  Serial.print("SPI Clock Divider: ");
  Serial.println(divider);
  Serial.print("SPI Speed: ");
  Serial.print(16000000 / divider);
  Serial.println(" Hz");
}