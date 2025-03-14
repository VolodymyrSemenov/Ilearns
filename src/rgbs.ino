/*
#include <FastLED.h>
#include <Arduino.h>

#define LED_PIN_LETTERS 6
#define LED_PIN_NUMBERS 7
#define WIDTH_PER_PIECE 3

// Define the amount of letters and numbers
const int num_letters = 26;
const int num_numbers = 21;
const int num_letter_leds = num_letters * WIDTH_PER_PIECE;
const int num_number_leds = num_numbers * WIDTH_PER_PIECE;
const int num_letter_leds_on = num_letters * (WIDTH_PER_PIECE-1);
const int num_number_leds_on = num_numbers * (WIDTH_PER_PIECE-1);
const int num_total_leds = 100;
// const int num_letter_leds = sizeof(letters) / sizeof(letters[0]);
// const int num_number_leds = sizeof(numbers) / sizeof(numbers[0]);


// Define the struct to store letter data
struct gamePiece {
  char character[3]; // The letter (a-z, 0-20)
  uint8_t uid[7]; // Unique ID for the game piece
  int decoder_value; // Decoder value of the rfid reader
  int positions[WIDTH_PER_PIECE]; // Array to store LED positions
};

// Structs for both the letters and numbers
gamePiece letters[num_letters];
gamePiece numbers[num_numbers];

// Lights default off
CRGB default_CRGB = CRGB::Black;
CRGB orange_1 = CRGB(250, 40, 0);

// Arrays of crgb values for each piece
CRGB letter_crgb_leds[num_letters];
CRGB number_crgb_leds[num_numbers];
CRGB leds[2];


// Initialize FastLED
void setup() {
  Serial.begin(9600);
  Serial.println("H1");
  delay(500);
  FastLED.addLeds<WS2811, LED_PIN_LETTERS, RGB>(letter_crgb_leds, num_letter_leds);
  Serial.println("H12");
  delay(500);
  FastLED.addLeds<WS2811, LED_PIN_NUMBERS, RGB>(number_crgb_leds, num_number_leds);
  Serial.println("H13");
  delay(500);
  //FastLED.clear();
  Serial.println("H14");
  delay(500);
  FastLED.show();
  Serial.println("H15");
  delay(500);
  FastLED.setBrightness(128);
  Serial.println("H16");
  delay(500);

  Serial.println("Hello1 ws2811");
  delay(500);
  generate_letter_gamepiece_structures(letters);  // Generate LED alphabet positions dynamically
  Serial.println("Hello2 ws2811");
  delay(500);
  generate_number_gamepiece_structures(numbers);  // Generate LED number positions dynamically
  Serial.println("Hello3 ws2811");
  delay(500);

  Serial.println("Hello");

  printGamePieces();
}

// Fills gamepiece structure for each letter
void generate_letter_gamepiece_structures(gamePiece letters[]) {
  char letters_list[num_letter_leds][3] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};    
  
  // Generate the letter game pieces
  for (int i = 0; i < num_letter_leds; i++) {
      Serial.println(i);
      delay(250);
      Serial.println(letters[i].character);
      delay(250);
      strcpy(letters[i].character, letters_list[i]);
      memset(letters[i].uid, 0, sizeof(letters[i].uid));
      letters[i].decoder_value = i;
      letter_crgb_leds[i] = default_CRGB;
      generatePositions(i * WIDTH_PER_PIECE, letters[i].positions);

  }
}

// Fills gamepiece structure for each number
void generate_number_gamepiece_structures(gamePiece numbers[]) {
  char numbers_list[num_number_leds][3] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"};
  
  // Generate the number game pieces
  for (int i = 0; i < num_number_leds; i++) {
    strcpy(numbers[i].character, numbers_list[i]);
    memset(numbers[i].uid, 0, sizeof(numbers[i].uid));
    numbers[i].decoder_value = num_letter_leds - i;
    number_crgb_leds[i] = default_CRGB;
    generatePositions(i * WIDTH_PER_PIECE, numbers[i].positions);
  }
}

// Function to generate positions for a letter/number based on index
void generatePositions(int baseIndex, int positions[]) {
  for (int i = 0; i < WIDTH_PER_PIECE; i++) {
    positions[i] = baseIndex + i;
    Serial.println(positions[i]);
  }
}

// Prints the letter and number game pieces structs
void printGamePieces() {
  // Print letter game pieces
  for (int i = 0; i < num_letter_leds; i++) {
    Serial.println();
    printGamePiece(letters[i]);
  }

  // Print number game pieces
  for (int i = 0; i < num_number_leds; i++) {
    Serial.println();
    printGamePiece(numbers[i]);
  }
}

// Helper function prints a single item in the struct
void printGamePiece(const gamePiece &piece) {
  // Print the game piece character
  Serial.print("Character: ");
  Serial.println(piece.character);

  // Print the positions array
  Serial.print("Positions: ");
  for (int i = 0; i < WIDTH_PER_PIECE; i++) {
    Serial.print(piece.positions[i]);
    if (i < WIDTH_PER_PIECE - 1) Serial.print(", ");
  }
  Serial.println();
}

// Display a letter with a specific color
void set_and_display_gamepiece_color(char letter[3], CRGB color, gamePiece game_piece_array[], CRGB leds[]) {

  // Find the letter in the array and set its LEDs
  for (int i = 0; i < sizeof(game_piece_array); i++) {
    if (game_piece_array[i].character == letter) {
      leds[i] = color;
      FastLED.show();  // Update the LEDs
      break;  // Exit the loop once the letter is found
    }
  }
}

// Creates a rainbow for the letters
void letter_fill_rainbow(){
  fill_rainbow(letter_crgb_leds, num_letter_leds, 0, 255/num_letter_leds);
}

// Creates a rainbow for the numbers
void number_fill_rainbow(){
  CRGB number_rainbow_colors[num_number_leds];
  fill_rainbow(number_rainbow_colors, num_number_leds, 0, 255/num_number_leds);
  for (int i = 0; i < num_number_leds; i++) {
    for (int j = 0; j < WIDTH_PER_PIECE; j++) {
      leds[numbers[i].positions[j]] = number_rainbow_colors[i];
    }
  }
  FastLED.show();  // Update the LEDs
  fill_rainbow(leds, num_number_leds, 0, 255/num_number_leds);
}

// Function to fade the lights of all letters with a traveling rainbow gradient
void fadeLettersWithRainbowGradient() {
  int totalLEDs = num_letter_leds * WIDTH_PER_PIECE;
  fill_rainbow(leds, totalLEDs, 0, 255 / totalLEDs);
  FastLED.show();  // Update the LEDs
}

void loop() {
  // fadeLettersWithRainbowGradient();  // Fade all letters with a traveling rainbow gradient
  Serial.println("Hello World");
  delay(1000);  // Wait 1 second
}
*/