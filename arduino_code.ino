// Include Libraries
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DRV8461Stepper.h>
#include <stdint.h>
#include <iostream>
#include <cstdlib>
using namespace std;

// Defining pins + settings for SPI and Serial
#define RX_PIN 10
#define TX_PIN 11
#define BAUD_RATE 9600

const uint_fast8_t CSPin1 = 10;
const uint_fast8_t CSPin2 = 9;

//  Internal Variables
uint_fast32_t RA_current = 0;
uint_fast32_t DEC_current = 0;
char RADEC_combined[12]
uint_fast8_t  TrackingMode = 0;     // 0 = Disabled, 1 = Enabled, 2 = Sidereal
uint_fast8_t  TrackingRate = 0;
uint_fast8_t  RA_stepperRate = 0;
uint_fast8_t  DEC_stepperRate = 0;
uint_fast8_t  RA_slewRate = 0;
char status[] = "fHptk";            // Problem:   lowercase = False, UPPERCASE = True
char command = '!';                 // Null command

// CHAR
const uint_fast8_t numChars = 32;   // Maximum number of characters in each communication
char receivedChars[numChars];
char data[numChars];

boolean newDataChar = false;



/*
//BIN
const byte numDataBytes = 8;
const byte numDataBits = numDataBytes * 8;
byte receivedBytes[numDataBytes];
byte numReceived = 0;

boolean newData = false;
*/

SoftwareSerial serial(RX_PIN, TX_PIN);

void setup() {

  // Initialize serial communication
    serial.begin(BAUD_RATE);

  SPI.begin();
}

void loop() {

  recvWithStartEndMarkers();      ///CHAR          Stores received data to "receivedChars"
  extractCommand();
  convertFunction();

  /*
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
  */
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




/*
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
*/
/*
void processData() {                                    ///BIN
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
*/



void extractCommand(){
  if (newDataChar == true){
    command = receivedChars[0];
    newDataChar = false;
  }
}

void extractData(){
  for (int i = 1; receivedChars[i] != '\0'; i++){
    data[i - 1] = receivedChars[i];
  }
}


void showNewData() {
    if (newDataChar == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newDataChar = false;
    }
}


void convertFunction() {
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
      
  }
}

// Each function defined.

// Functions that communicate with the Raspberry Pi:
void Handshake() {
  // Code for Handshake action
  status[1] = 'h';          // Disables Handshake alert
  serial.println('S');      // Returns 'S', which is the expected return from the handshake.

}

void OutputStatus() {
  // Code for OutputStatus action
  serial.println(status);   // Prints the status to the serial console.
}

void SetTrackMode() {
  // Code for SetTrackMode action
  TrackingMode = atoi(data); // Converts characterarray to integer.
}

void SetTrackEnabled() {
  // Code for SetTrackEnabled action
  TrackingMode = 1;
}

void SetTrackRate() {
  // Code for SetTrackRate action
  TrackingRate = atoi(data);
}

void Sync() {
  // Code for Sync action
  // Data format: hhmmssddmmss    
  // RA   format: hhmmss  (6) (0-5)
  // DEC  format: ddmmss  (6) (6-11)
  char RA_gather[6];
  char RA_hours[2];
  char RA_mins[2];
  char RA_secs[2];

  char DEC_gather[6];
  char DEC_degrees[2];
  char DEC_arcmins[2];
  char DEC_arcsecs[2];

  for (int i = 0; i != 6; i++){       // Gather RA terms
    RA_gather[i] = data[i];
  }
  for (int i = 0; i != 2; i++){          // Gather Hours
    RA_hours[i] = RA_gather[i];
  }
  for (int i = 2; i != 4; i++){          // Gather Minutes
    RA_mins[i - 2] = RA_gather[i];
  }
  for (int i = 4; i != 6; i++){          // Gather Seconds
    RA_secs[i - 4] = RA_gather[i];
  }

  for (int i = 6; i != 12; i++){      // Gather DEC terms
    DEC_gather[i - 6] = data[i];
  }
  for (int i = 0; i != 2; i++){          // Gather Hours
    DEC_degrees[i] = DEC_gather[i];
  }
  for (int i = 2; i != 4; i++){          // Gather Minutes
    DEC_arcmins[i - 2] = DEC_gather[i];
  }
  for (int i = 4; i != 6; i++){          // Gather Seconds
    DEC_arcsecs[i - 4] = DEC_gather[i];
  }

  RA_current = atoi(RA_hours) * 3600 + atoi(RA_mins) * 60 + atoi(RA_secs);
  DEC_current = atoi(DEC_degrees) * 3600 + atoi(DEC_arcmins) * 60 + atoi(DEC_arcsecs);
}

void updateLocation() {
  // Code for updateLocation action
}

void returnCoords(){
  // Code for returning coordinates

  // In integers:
  uint_fast32_t RA_hours;
  uint_fast32_t RA_mins;
  uint_fast32_t RA_secs;

  uint_fast32_t DEC_degrees;
  uint_fast32_t DEC_arcmins;
  uint_fast32_t DEC_arcsecs;

  // In chararrays:
  char RA_gather_c[6];
  char RA_hours_c[2];
  char RA_mins_c[2];
  char RA_secs_c[2];

  char DEC_gather_c[6];
  char DEC_degrees_c[2];
  char DEC_arcmins_c[2];
  char DEC_arcsecs_c[2];

  // Right Ascension
  RA_hours = RA_current/3600;                               //hh
  RA_mins = RA_current/60 - RA_hours * 60;                  //mm
  RA_secs = RA_current - RA_hours * 3600 - RA_mins * 60;    //ss

  sprintf(RA_hours_c, "%d", RA_hours);
  sprintf(RA_mins_c, "%d", RA_mins);
  sprintf(RA_secs_c, "%d", RA_secs);

  if (RA_hours_c.size == 2) {
    for (int i = RA_hours_c.size - 1; i > 0; i--) {
      RA_hours_c[i] = RA_hours_c[i - 1];
    }
    RA_hours_c[0] = 0;
  }

  if (RA_mins_c.size == 2) {
    for (int i = RA_mins_c.size - 1; i > 0; i--) {
      RA_mins_c[i] = RA_mins_c[i - 1];
    }
    RA_mins_c[0] = 0;
  }

  if (RA_secs_c.size == 2) {
    for (int i = RA_secs_c.size - 1; i > 0; i--) {
      RA_secs_c[i] = RA_secs_c[i - 1];
    }
    RA_secs_c[0] = 0;
  }
 
  // Declination
  DEC_degrees = DEC_current/3600;
  DEC_arcmins = DEC_current/60 - DEC_degrees * 60;
  DEC_arcsecs = DEC_current - DEC_degrees * 3600 - DEC_arcmins * 60;

  sprintf(DEC_degrees_c, "%d", DEC_degrees);
  sprintf(DEC_arcmins_c, "%d", DEC_arcmins);
  sprintf(DEC_arcsecs_c, "%d", DEC_arcsecs);

  if (DEC_degrees_c.size == 2) {
    for (int i = DEC_degrees_c.size - 1; i > 0; i--) {
      DEC_degrees_c[i] = DEC_degrees_c[i - 1];
    }
    DEC_degrees_c[0] = 0;
  }

  if (DEC_arcmins_c.size == 2) {
    for (int i = DEC_arcmins_c.size - 1; i > 0; i--) {
      DEC_arcmins_c[i] = DEC_arcmins_c[i - 1];
    }
    DEC_arcmins_c[0] = 0;
  }

  if (DEC_arcsecs_c.size == 2) {
    for (int i = DEC_arcsecs_c.size - 1; i > 0; i--) {
      DEC_arcsecs_c[i] = DEC_arcsecs_c[i - 1];
    }
    DEC_arcsecs_c[0] = 0;
  }

  for (int i = 0; i < 6; i++){                         // Merges all of the RA + DEC into one RA_gather_c and DEC_gather_c
    if (0 <= i < 2) {
      RA_gather_c[i] = RA_hours_c[i];
      DEC_gather_c[i] = DEC_degrees_c[i];
    }
    else if (2 <= i < 4) {
      RA_gather_c[i] = RA_mins_c[i - 2];
      DEC_gather_c[i] = DEC_arcmins_c[i - 2];
    }
    else if (4 <= i < 6) {
      RA_gather_c[i] = RA_secs_c[i - 4];
      DEC_gather_c[i] = DEC_arcsecs_c[i - 4];
    }
  }

  for (int i = 0; i < 12; i++){
    if (0 <= i < 6){
      RADEC_combined[i] = RA_gather_c[i];
    }
    else if (6 <= i < 12){
      RADEC_combined[i] = DEC_gather_c[i - 6];
    }
  }

}


// Functions that communicate with the DRV8461:
void MoveNS() {
  // Code for MoveNS action

}

void MoveWE() {
  // Code for MoveWE action

}

void Abort() {
  // Code for Abort action

}

void StepNorth() {
  // Code for StepNorth action

}

void StepSouth() {
  // Code for StepSouth action

}

void StepEast() {
  // Code for StepEast action
}

void StepWest() {
  // Code for StepWest action
}

void Goto() {
  // Code for Goto action
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
