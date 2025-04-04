/**************************************************************************
 * Audio Configuration
 * 
 * This file configures audio playback from a USB drive using the AdvancedDAC.
 * It opens and streams a WAV file corresponding to a given letter or number,
 * based on the game state:
 * 
 *   GameState 1: Number Pointing – plays a file from USBNumberPoint.
 *   GameState 2: Letter Pointing – plays a file from USBLetterPoint.
 *   GameState 3: Letter Sounding  – plays a file from USBLetterSound.
 * 
 **************************************************************************/

 #include <Arduino_AdvancedAnalog.h>
 #include <DigitalOut.h>
 #include <Arduino_USBHostMbed5.h>
 #include <FATFileSystem.h>
 
 AdvancedDAC dac0(A12);  // Adjust the DAC pin as needed.
 
 USBHostMSD msd;
 mbed::FATFileSystem usb("usb");
 
 FILE *file = nullptr;
 int sample_size = 0;
 int samples_count = 0;
 bool played = false;  // Set to true when audio playback is complete.
 
 // Define the USB paths for each type of audio file.
 String USBLetterPoint = "/usb/Point-Letter-Split-wav/pLett";
 String USBLetterSound = "/usb/Letter-Sound-Split-wav/sLett";
 String USBNumberPoint = "/usb/Point-Number-Split-wav/pNum";
 String wav = ".wav";
 
 // Opens and configures the audio file for the given input.
 // For gameState 1 (Number Pointing), tempInput should be a digit character (or you may extend to a String if needed).
 // For gameState 2 (Letter Pointing) or 3 (Letter Sounding), tempInput is the letter.
 void configFile(int gameState, String tempInput) {
   String result = "";
   switch (gameState) {
     case 1:  // Number Pointing
       result = USBNumberPoint + tempInput + wav;
       break;
     case 2:  // Letter Pointing
       result = USBLetterPoint + tempInput + wav;
       break;
     case 3:  // Letter Sounding
       result = USBLetterSound + tempInput + wav;
       break;
     default:
       break;
   }
   file = fopen(result.c_str(), "rb");
   if (file == nullptr) {
     Serial.print("Error opening audio file: ");
     Serial.println(strerror(errno));
     return;
   }
   Serial.println("Opening audio file: " + result);
 
   // Read WAV header.
   struct wav_header_t {
     char chunkID[4];
     unsigned long chunkSize;
     char format[4];
     char subchunk1ID[4];
     unsigned long subchunk1Size;
     unsigned short audioFormat;
     unsigned short numChannels;
     unsigned long sampleRate;
     unsigned long byteRate;
     unsigned short blockAlign;
     unsigned short bitsPerSample;
   };
 
   wav_header_t header;
   fread(&header, sizeof(header), 1, file);
 
   // Skip to the "data" chunk.
   struct chunk_t {
     char ID[4];
     unsigned long size;
   } chunk;
   while (true) {
     fread(&chunk, sizeof(chunk), 1, file);
     if (*(unsigned int *)&chunk.ID == 0x61746164)  // "data" in little-endian
       break;
     fseek(file, chunk.size, SEEK_CUR);
   }
 
   sample_size = header.bitsPerSample / 8;
   samples_count = chunk.size * 8 / header.bitsPerSample;
 
   if (!dac0.begin(AN_RESOLUTION_12, header.sampleRate, 256, 16)) {
     Serial.println("Failed to start DAC!");
     return;
   }
 }
 
 // Reads and plays a block of audio data via the DAC.
 // Call repeatedly until the file is completely played.
 void playAudioFile() {
   if (dac0.available() && !feof(file)) {
     uint16_t sample_data[256] = {0};
     size_t readCount = fread(sample_data, sample_size, 256, file);
     SampleBuffer buf = dac0.dequeue();
     for (size_t i = 0; i < buf.size(); i++) {
       uint16_t dac_val = ((static_cast<unsigned int>(sample_data[i]) + 32768) >> 4) & 0x0fff;
       buf[i] = dac_val;
     }
     dac0.write(buf);
     if (feof(file)) {
       fclose(file);
       played = true;
       Serial.println("Audio playback complete.");
     }
   }
 }
 
 // Plays the audio file corresponding to the given letter (or number).
 // This function blocks until playback is complete.
 void playLetterAudio(String letter, int gameState = 2) {
   // For letter games, gameState 2 (pointing) or 3 (sounding) are used.
   // Here, we use the first character of the letter string.
   String tempInput = letter;
   configFile(gameState, tempInput);
   played = false;
   while (!played) {
     playAudioFile();
     delay(1);
   }
 }
 