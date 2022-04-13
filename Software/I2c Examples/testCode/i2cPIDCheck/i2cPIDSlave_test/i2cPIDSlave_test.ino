#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Wire.h>

#define OUT_PAYLOAD_SIZE  5
#define ADDRESS           2

// pin definition
const int encoderAPin = 2;
const int encoderBPin = 3;
const int motorDirPin = 4;
const int motorPWMPin = 5;

//  parameters for velocity calculations
int clicksPerOutRev = 1920;   // from Pololu specifications
long sampTime       = 500000; // number of microseconds to count clicks

// containers
byte OutPayload[OUT_PAYLOAD_SIZE];
byte dir           = 0;
byte pwmWheel      = 0;
float wheelAngVel  = 0;
long lastMillis    = 0;

// define objects
Encoder enc(encoderAPin, encoderBPin);

void setup() {
  // initialize pins
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorPWMPin, OUTPUT);

  // start I2C communication
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  // log time at start of first loop
  lastMillis = millis();
}

void loop() {
  // process encoder input
  if ((millis() - lastMillis) > sampTime) {
    // calculate wheel angular velocity (neglecting slip)
    wheelAngVel = enc.read() * 60000.00 / (clicksPerOutRev * (millis() - lastMillis));

    // reset time, encoder
    lastMillis = millis();
    enc.write(0);
  }

  // write outputs to motor
  digitalWrite(motorDirPin, dir);
  digitalWrite(motorPWMPin, pwmWheel) ;
}

// when master asks for data
void requestEvent() {
  // get direction
  byte measDir;
  if (wheelAngVel < 0) {
    wheelAngVel = -wheelAngVel;
    OutPayload[0] = 0;
  }
  else {
    OutPayload[0] = 1;
  }

  // union structure to convert floating point wheel velocity to byte array
  union float2ByteArray {
      byte ByteArray[4];
      float floatVal;
  } u;
  u.floatVal = wheelAngVel;
  OutPayload[1] = u.ByteArray[0];
  OutPayload[2] = u.ByteArray[1];
  OutPayload[3] = u.ByteArray[2];
  OutPayload[4] = u.ByteArray[3];

  // send payload to master
  Wire.write(OutPayload, OUT_PAYLOAD_SIZE);
}

// when master wants to give data
void receiveEvent(int byteCount) {
  dir      =  Wire.read(); // first byte is direction
  pwmWheel =  Wire.read(); // second byte is PWM
}