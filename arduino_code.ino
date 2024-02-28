#include <SoftwareSerial.h>
#include <SPI.h>
#include <DRV8461Stepper.h>

#define RX_PIN 10
#define TX_PIN 11
#define BAUD_RATE 9600

const uint8_t CSPin1 = 10;
const uint8_t CSPin2 = 9;

//BIN
const byte numDataBytes = 8;
const byte numDataBits = numDataBytes * 8;
byte receivedBytes[numDataBytes];
byte numReceived = 0;

boolean newData = false;

/// CHAR
const byte numChars = 32;
char receivedChars[numChars];

boolean newDataChar = false;



SoftwareSerial serial(RX_PIN, TX_PIN);

void setup() {

  // Initialize serial communication
    serial.begin(BAUD_RATE);

  SPI.begin();
}

void loop() {

  recvWithStartEndMarkers();      ///CHAR
  showNewData();

  recvBytesWithStartEndMarkers();   /// BIN
  processData();

  // Check if data is available to read
  if (serial.available()) {
    // Read the incoming byte
    uint8_t input = serial.read();

    // Not the last 4 bits contain action type.
    uint8_t action = input >> numDataBits;
    uint8_t parameter = input^(action << numDataBits)
    
    // Process the input
    processInput(action);
  }
}

void recvWithStartEndMarkers() {                 ////CHAR
    static boolean recvInProgress = false;
    static byte ndx = 0;
    const char startMarker = '<';
    const char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newDataChar == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newDataChar = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}





void recvBytesWithStartEndMarkers() {             ////BIN
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3C;
    byte endMarker = 0x3E;
    byte rb;
   

    while (Serial.available() > 0 && newData == false) {
        rb = Serial.read();

        if (recvInProgress == true) {
            if (rb != endMarker) {
                receivedBytes[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                }
            }
            else {
                receivedBytes[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }

        else if (rb == startMarker) {
            recvInProgress = true;
        }
    }
}


void showNewData() {
    if (newDataChar == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newDataChar = false;
    }
}


void processData() {
    if (newData == true) {
        Serial.print("This just in (HEX values)... ");
        for (byte n = 0; n < numReceived; n++) {
            // Not the last 4 bits contain action type.
            uint8_t action = input >> numDataBits;
            uint8_t parameter = input^(action << numDataBits)
            
            // Process the input
            processInput(action);
        }
        Serial.println();
        newData = false;
    }
}

void extractCommand(){
  if (newDataChar == true){
    return receivedChars[0]
  }
}


void processInput(char command) {
  // Define your actions based on input values

  switch(command) {
    case 'H': //00000001XXXX  'Handshake'
      Handshake();
      break;
    case 'O': //00000010XXXX  'Output'
      OutputStatus();
      break;
    case 'D': //00000011XXXX  'Declination'
      MoveNS();
      break;
    case 'R': //00000100XXXX  'Right Ascension'
      MoveWE();
      break; 
    case 'A': //00000101XXXX  'Abort'
      Abort();
      break;
    case 'N': //00000110XXXX  'North'
      StepNorth();
      break;
    case 'S': //00000111XXXX  'South'
      StepSouth();
      break;
    case 'E': //00001000XXXX  'East'
      StepEast();
      break;
    case 'W': //00001001XXXX  'West'
      StepWest();
      break;
    case 'T': //00001010XXXX  'Tracking mode'
      SetTrackMode();
      break;
    case 't': //00001011XXXX  '"t"racking enabled'
      SetTrackEnabled();
      break;
    case 'r': //00001100XXXX  '"r"ate of tracking'
      SetTrackRate();
      break;
    case 'G': //00001101XXXX  'Goto'                       Moves telescope to a specified RA/DEC
      Goto();
      break;
    case 's': //00001110XXXX  '"s"ync'                     Updates mount coordinates
      Sync();
      break;
    case 'L': //00001111XXXX  'Location'                   Update GPS location
      updateLocation();
      break;
    case 'P': //00010000XXXX  'Park'
      Park();
      break;
    case 'U': //00010001XXXX  'Unpark'
      Unpark();
      break;
    case 'c': //00010010XXXX  '"c"urrent park'
      SetCurrentPark();
      break;
    case 'd': //00010011XXXX  '"d"efault park'
      SetDefaultPark();
      break;
    case 'C': //              'Coordinates'
      returnCoords();
      break;
        
    default:
      Serial.println("Unknown input");
  }
}

//Actions defined
void Handshake() {
  // Code for Handshake action
  


}

void OutputStatus() {
  // Code for OutputStatus action
  Serial.println("Calling OutputStatus function");
}

void MoveNS() {
  // Code for MoveNS action
  Serial.println("Calling MoveNS function");
}

void MoveWE() {
  // Code for MoveWE action
  Serial.println("Calling MoveWE function");
}

void Abort() {
  // Code for Abort action
  Serial.println("Calling Abort function");
}

void StepNorth() {
  // Code for StepNorth action
  Serial.println("Calling StepNorth function");
}

void StepSouth() {
  // Code for StepSouth action
  Serial.println("Calling StepSouth function");
}

void StepEast() {
  // Code for StepEast action
}

void StepWest() {
  // Code for StepWest action
}

void SetTrackMode() {
  // Code for SetTrackMode action
}

void SetTrackEnabled() {
  // Code for SetTrackEnabled action
}

void SetTrackRate() {
  // Code for SetTrackRate action
}

void Goto() {
  // Code for Goto action
}

void Sync() {
  // Code for Sync action
}

void updateLocation() {
  // Code for updateLocation action
}

void Park() {
  // Code for Park action
}

void Unpark() {
  // Code for Unpark action
}

void SetCurrentPark() {
  // Code for SetCurrentPark action
}

void SetDefaultPark() {
  // Code for SetDefaultPark action
}

void returnCoords(){
  // Code for returning coordinates
}