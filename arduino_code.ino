#include <SoftwareSerial.h>
#include <SPI.h>
#include <DRV8461Stepper.h>

#define RX_PIN 10
#define TX_PIN 11
#define BAUD_RATE 9600

const uint8_t CSPin1 = 10;
const uint8_t CSPin2 = 9;



SoftwareSerial serial(RX_PIN, TX_PIN);

void setup() {

  // Initialize serial communication
  Serial.begin(9600);
  serial.begin(BAUD_RATE);

  SPI.begin();
}

void loop() {
  // Check if data is available to read
  if (serial.available()) {
    // Read the incoming byte
    uint8_t input = serial.read();

    // Not the last 4 bits contain action type.
    uint8_t action = input >> 4;
    uint8_t parameter = input^(action << 4)
    
    // Process the input
    processInput(action);
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

// Define your action functions here
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