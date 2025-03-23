  //Set all LED's off | set all LED's to red
  //pass random number into audio
  //match number with rfid tag
  //check if reader is reading the correct rfid tag

struct matchRFID{
  String number;
  String rfidTag;
};

matchRFID RFIDDict[] = {
  {"1", "tag1"},
  {"2", "tag2"},
  {"3", "tag3"},
  {"4", "tag4"},
  {"5", "tag5"},
  {"6", "tag6"},
  {"7", "tag7"},
  {"8", "tag8"},
  {"9", "tag9"},
  {"10", "tag10"},
  {"11", "tag11"},
  {"12", "tag12"},
  {"13", "tag13"},
  {"14", "tag14"},
  {"15", "tag15"},
  {"16", "tag16"},
  {"17", "tag17"},
  {"18", "tag18"},
  {"19", "tag19"},
  {"20", "tag20"}
};


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  int gameState = 3;     //gameState 3 = number pointing

  //turn off all led
  

}

void loop() {
  begin_game_find_numbers();
  delay(3000);
}

String getRandomNumber(){
  int num = 1 + (random() % 20);
  return String(num);
}

bool checkRFIDTagMatch(String currentNumber, String readRfidTag){
  for(int i = 0; i < 26; i++){
    if(RFIDDict[i].number == currentNumber){
      if(RFIDDict[i].rfidTag == readRfidTag){
        return true;
      }else{
        return false;
      }
    }
  }
}

void begin_game_find_numbers() {
  String randomNumber = getRandomNumber(); 
  Serial.println(randomNumber);

  //find rfid tag mapped to the numb
  
  //configFile(gameState, randomNumber);    //play audio file once

  String tagBeingRead = "";                 //store tag being read from reader

  bool checkResult = checkRFIDTagMatch(randomNumber, "tag2");    //check if rfid tag being read corresponds with current number
  if(checkResult == true){
    Serial.println("checkResult == true");
    //turn on green LED
  }
  
}
