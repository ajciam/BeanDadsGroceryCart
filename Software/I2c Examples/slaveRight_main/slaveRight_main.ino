#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Wire.h>

#define OUT_PAYLOAD_SIZE  5
#define ADDRESS           3

// pin definition
const int encoderAPin = 3;
const int encoderBPin = 2;
const int motorDirPin = 4;
const int motorPWMPin = 5;

//  parameters for velocity calculations
int clicksPerOutRev = 1920; // from Pololu specifications
long sampTime       = 15;  // number of ms to count clicks

// containers
bool dir        = 0;
int pwmWheel    = 0;
float wheelRPM  = 0;
long lastMillis = 0;
long heartBeatTime = 0;

union {
  byte ByteArray[4];
  float fval;
} u;

// define objects
Encoder enc(encoderAPin, encoderBPin);

void setup() {
  // initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorPWMPin, OUTPUT);

  // start I2C communication
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  // log time at start of first loop
  lastMillis = millis();
  enc.write(0);
}

void loop() {
  // process encoder input
  if ((millis() - lastMillis) > sampTime) {
    // calculate wheel angular velocity (neglecting slip)
    wheelRPM = enc.read() * 60000.00 / (clicksPerOutRev * (millis() - lastMillis));

    // reset time, encoder
    lastMillis = millis();
    enc.write(0);
  }

  // write outputs to motor
  digitalWrite(motorDirPin, dir);
  analogWrite(motorPWMPin, pwmWheel);

  if (millis() - heartBeatTime > 500) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    heartBeatTime = millis();
  }
  
}

// when master asks for data
void requestEvent() {
  // payload container
  byte OutPayload[OUT_PAYLOAD_SIZE];

  // get direction
  byte measDir;
  if (wheelRPM < 0) {
    wheelRPM      = -1 * wheelRPM;
    OutPayload[0] = 0;
  }
  else {
    OutPayload[0] = 1;
  }

  // convert wheel velocity float to byte array
  u.fval = wheelRPM;
  for (int i = 0; i < OUT_PAYLOAD_SIZE - 1; i++){
    OutPayload[i + 1] = u.ByteArray[i];
  }

  // send payload to master
  Wire.write(OutPayload, OUT_PAYLOAD_SIZE);
}

// when master wants to give data
void receiveEvent(int byteCount) {
  dir      =  Wire.read(); // first byte is direction
  pwmWheel =  Wire.read(); // second byte is PWM
}
