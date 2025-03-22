  //Set all LED's off | set all LED's to red
  //pass random letter into audio
  //match letter with rfid tag
  //check if reader is reading the correct rfid tag

struct matchRFID{
  char letter;
  String rfidTag;
};

matchRFID RFIDDict[] = {
  {'A', "tag1"},
  {'B', "tag2"},
  {'C', "tag3"},
  {'D', "tag4"},
  {'E', "tag5"},
  {'F', "tag6"},
  {'G', "tag7"},
  {'H', "tag8"},
  {'I', "tag9"},
  {'J', "tag10"},
  {'K', "tag11"},
  {'L', "tag12"},
  {'M', "tag13"},
  {'N', "tag14"},
  {'O', "tag15"},
  {'P', "tag16"},
  {'Q', "tag17"},
  {'R', "tag18"},
  {'S', "tag19"},
  {'T', "tag20"},
  {'U', "tag21"},
  {'V', "tag22"},
  {'W', "tag23"},
  {'X', "tag24"},
  {'Y', "tag25"},
  {'Z', "tag26"}
};


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  int gameState = 2;     //gameState 2 = letter pointing

  //turn off all led
  

}

void loop() {
  begin_game_find_letters_spoken();
  delay(3000);
}

char getRandomLetter(){
  return 'A' + (random() % 26);
}

bool checkRFIDTagMatch(char currentLetter, String readRfidTag){
  for(int i = 0; i < 26; i++){
    if(RFIDDict[i].letter == currentLetter){
      if(RFIDDict[i].rfidTag == readRfidTag){
        return true;
      }else{
        return false;
      }
    }
  }
}

void begin_game_find_letters_spoken() {
  char randomLetter = getRandomLetter(); 
  Serial.println(randomLetter);

  //find rfid tag mapped to the letter
  
  //configFile(gameState, randomLetter);    //play audio file once

  String tagBeingRead = "";                 //store tag being read from reader

  bool checkResult = checkRFIDTagMatch(randomLetter, "tag20");    //check if rfid tag being read corresponds with current letter
  if(checkResult == true){
    Serial.println("checkResult == true");
    //turn on green LED
  }
  
}
