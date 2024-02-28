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



void loop() {
    recvWithStartEndMarkers();
    showNewData();
}





SoftwareSerial serial(RX_PIN, TX_PIN);

void setup() {

  // Initialize serial communication
  serial.begin(BAUD_RATE);
  // Show Arduino is ready
  Serial.println("<Arduino is ready>");

  SPI.begin();
}

void loop() {

  recvWithStartEndMarkers();      ///CHAR

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
    char startMarker = '<';
    char endMarker = '>';
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


void processInput(uint8_t action) {
  // Define your actions based on input values

  switch(action) {
    case 1: //00000001XXXX
      Handshake();
      break;
    case 2: //00000010XXXX
      OutputStatus();
      break;
    case 3: //00000011XXXX
      MoveNS();
      break;
    case 4: //00000100XXXX
      MoveWE();
      break; 
    case 5: //00000101XXXX
      Abort();
      break;
    case 6: //00000110XXXX
      StepNorth();
      break;
    case 7: //00000111XXXX
      StepSouth();
      break;
    case 8: //00001000XXXX
      StepEast();
      break;
    case 9: //00001001XXXX
      StepWest();
      break;
    case 10: //00001010XXXX
      SetTrackMode();
      break;
    case 11: //00001011XXXX
      SetTrackEnabled();
      break;
    case 12: //00001100XXXX
      SetTrackRate();
      break;
    case 13: //00001101XXXX
      Goto();
      break;
    case 14: //00001110XXXX
      Sync();
      break;
    case 15: //00001111XXXX
      updateLocation();
      break;
    case 16: //00010000XXXX
      Park();
      break;
    case 17: //00010001XXXX
      Unpark();
      break;
    case 18: //00010010XXXX
      SetCurrentPark();
      break;
    case 19: //00010011XXXX
      SetDefaultPark();
      break;
    
    default:
      Serial.println("Unknown input");
  }
}

//Actions defined
void Handshake() {
  // Code for Handshake action
  Serial.println("Calling Handshake function");
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