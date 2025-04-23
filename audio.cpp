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

AdvancedDAC dac0(A12);

USBHostMSD msd;
mbed::FATFileSystem usb("usb");

FILE *file = nullptr;
int sample_size = 0;
int samples_count = 0;

String tempLetter = "10";
String currentAudio = "";

 // Define the USB paths for each type of audio file.
String USBLetterPoint = "/usb/Point-Letter-Split-wav/pLett";
String USBLetterSound = "/usb/Letter-Sound-Split-wav/sLett";
String USBNumberPoint = "/usb/Point-Number-Split-wav/pNum";
String wav = ".wav";

int gameState = 1;
bool played = false;


void setup() {
  Serial.begin(115200);
  while (!Serial);

  /* Enable power for HOST USB connector. */

  pinMode(PA_15, OUTPUT);
  digitalWrite(PA_15, HIGH);

  Serial.println("Please connect a USB stick to the GIGA's USB port ...");
  while (!msd.connect()) delay(100);

  Serial.println("Mounting USB device ...");
  int const rc_mount = usb.mount(&msd);
  if (rc_mount) {
    Serial.print("Error mounting USB device ");
    Serial.println(rc_mount);
    return;
  }
  configFile(gameState, tempLetter);
}


//converts digital to analog
void loop() {
  if(played == false){
    playAudioFile();
  }

  if (Serial1.available()) {
    String toSound = Serial1.readString();
    int gameState = (int) toSound.charAt(0);
    String tempLetter = toSound.substring(1);
    Serial.println("Serial receiverd");
    delay(500);
    configFile(gameState, tempLetter);
  } 
}


void playAudioFile(){
  if (dac0.available() && !feof(file)) {
    /* Read data from file. */
    uint16_t sample_data[256] = { 0 };
    fread(sample_data, sample_size, 256, file);

    /* Get a free buffer for writing. */
    SampleBuffer buf = dac0.dequeue();

    /* Write data to buffer. */
    for (size_t i = 0; i < buf.size(); i++) {
      /* Scale down to 12 bit. */
      uint16_t const dac_val = ((static_cast<unsigned int>(sample_data[i]) + 32768) >> 4) & 0x0fff;
      buf[i] = dac_val;
    }

    /* Write the buffer to DAC. */
    dac0.write(buf);

    if(feof(file)){
      fclose(file);
      played = true;
      Serial.println("Finished Playing: " + currentAudio);
      configFile(gameState, tempLetter);
    }
  }
}


 // Opens and configures the audio file for the given input.
 // For gameState 1 (Number Pointing), tempInput should be a digit character (or you may extend to a String if needed).
 // For gameState 2 (Letter Pointing) or 3 (Letter Sounding), tempInput is the letter.
void configFile(int gameState, String tempLetter) {
  /* 16-bit PCM Mono 16kHz realigned noise reduction */
  String result = "";
  switch(gameState){
  case 1:  //Number Pointing
    result = USBNumberPoint + tempLetter + wav;
    file = fopen(result.c_str(), "rb");
    break;

  case 2:  //Letter Pointing
    //tempLetter++;
    result = USBLetterPoint + tempLetter + wav;
    file = fopen(result.c_str(), "rb");
    break;

  case 3:  //Letter Sounding
    result = USBLetterSound + tempLetter + wav;
    file = fopen(result.c_str(), "rb");
    break;

  default:
    
    break;
  }

  currentAudio = result;

  if (file == nullptr) {
    Serial.print("Error opening audio file: ");
    Serial.println(strerror(errno));
    return;
  }

  Serial.println("Opening audio file: " + result);   //Show current audio file

  // Read WAV header
  struct wav_header_t {
    char chunkID[4];              //"RIFF" = 0x46464952
    unsigned long chunkSize;      //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
    char format[4];               //"WAVE" = 0x45564157
    char subchunk1ID[4];          //"fmt " = 0x20746D66
    unsigned long subchunk1Size;  //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
  };

  wav_header_t header;
  fread(&header, sizeof(header), 1, file);

  /* Find the data section of the WAV file. */
  struct chunk_t {
    char ID[4];
    unsigned long size;
  };

  chunk_t chunk;
  /* Find data chunk. */
  while (true) {
    fread(&chunk, sizeof(chunk), 1, file);
    if (*(unsigned int *)&chunk.ID == 0x61746164)
      break;
    /* Skip chunk data bytes. */
    fseek(file, chunk.size, SEEK_CUR);
  }

  /* Determine number of samples. */
  sample_size = header.bitsPerSample / 8;
  samples_count = chunk.size * 8 / header.bitsPerSample;

  /* Configure the advanced DAC. */
  if (!dac0.begin(AN_RESOLUTION_12, header.sampleRate, 256, 16)) {
    //Serial.println("Failed to start DAC1 !");
    return;
  }
}
