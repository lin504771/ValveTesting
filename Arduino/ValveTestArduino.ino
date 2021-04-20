int solenoidPins[8] = {2, 4, 5, 7, 8, 9, 11, 13};

const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};

int valveStates[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//=============

void setup() {
  for(int i = 0; i < 8; i++) {
    pinMode(solenoidPins[i], OUTPUT);
  }
  Serial.begin(9600);
    // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {
  getDataFromPC();
  setValveStates();
  replyToPC();
}

//=============

void getDataFromPC() {

  // receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {

    char x = Serial.read();

    // the order of these IF clauses is significant
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============
 
void parseData() {

  // split the data into its parts
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer," ");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  if(strcmp(messageFromPC, "SV") == 0) {
     for(int i = 0; i < 8; i++) {
        strtokIndx = strtok(NULL, " ");
        valveStates[i] = atoi(strtokIndx);
     }
  }
}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    //IMPORTANT: this is the start character
    Serial.print("<");

    Serial.print("Command: ");
    Serial.print(messageFromPC);
    Serial.print(" |Current state: ");
    for(int i = 0; i < 8; i++) {
      Serial.print(valveStates[i]);
      Serial.print(" ");
    }
    
    //IMPORTANT: this is the end character
    Serial.println(">");
  }
}

void setValveStates() {
  for(int i = 0; i < 8; i++) {
    if(valveStates[i] == 0) {
      digitalWrite(solenoidPins[i], LOW);
    }
    else {
      digitalWrite(solenoidPins[i], HIGH);
    }
  }
}
