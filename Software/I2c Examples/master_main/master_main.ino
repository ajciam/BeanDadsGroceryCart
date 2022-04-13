#include <Wire.h>
#include <PID_v1.h>

// i2c communication
const int US_IN_PAYLOAD_SIZE   = 1; // incoming bytes from ultrasonic slave
const int ENC_IN_PAYLOAD_SIZE  = 5; // incoming bytes from encoder slave
const int ENC_OUT_PAYLOAD_SIZE = 2; // outgoing bytes to encoder slave
const int ULTRASONIC_ADDRESS   = 4; // address of ultrasonic slav
const int LEFT_WHEEL_ADDRESS   = 2; // address of left wheel slave
const int RIGHT_WHEEL_ADDRESS  = 3; // address of right wheel slave

// containers
char desiredOutputSwitch = 's';
long lastChange = 0;
union {
      byte byteArray[4];
      float fval;
} u;

// PID
double measLeftWheel, desLeftWheel, pwmLeftWheel;
double leftKp = 2.4;
double leftKi = 4;
double leftKd = .05;

double measRightWheel, desRightWheel, pwmRightWheel;
double rightKp = 2.4;
double rightKi = 4;
double rightKd = .05;

PID leftWheelAngVelPID(&measLeftWheel, &pwmLeftWheel, &desLeftWheel, leftKp, leftKi, leftKd, DIRECT);
PID rightWheelAngVelPID(&measRightWheel, &pwmRightWheel, &desRightWheel, rightKp, rightKi, rightKd, DIRECT);

long heartBeatTime = 0;


void setup() {
  // for debug
  Serial.begin(9600);

  // set limits on the PID outputs
  leftWheelAngVelPID.SetOutputLimits(-255,255);
  rightWheelAngVelPID.SetOutputLimits(-255,255);

  // start I2C
  Wire.begin();

  leftWheelAngVelPID.SetMode(AUTOMATIC);
  rightWheelAngVelPID.SetMode(AUTOMATIC);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if ((millis() - lastChange < 5000)) {
    desiredOutputSwitch = 'p';
  }
  if ((millis() - lastChange > 5000) && (millis() - lastChange < 15000)) {
    desiredOutputSwitch = 's';
  }
  else {
    desiredOutputSwitch = 'p';
  }
//  if ((millis() - lastChange > 30000) && (millis() - lastChange < 40000)) {
//    desiredOutputSwitch = 'b';
//  }
//  if ((millis() - lastChange > 40000) && (millis() - lastChange < 50000)) {
//    desiredOutputSwitch = 's';
//  }
//  if (millis() - lastChange > 50000) {
//    desiredOutputSwitch = 'p';
//  }

  if (Serial.available()) {
    desiredOutputSwitch = Serial.read();
  }
  
  // update wheel speeds from slave controllers;
  getWheelAngVel(LEFT_WHEEL_ADDRESS, measLeftWheel);
  getWheelAngVel(RIGHT_WHEEL_ADDRESS, measRightWheel);

  // state machine to quickly move between drive states
  switch(desiredOutputSwitch) {
    case 'f': // straight-line forward drive
      desLeftWheel  = 250;
      desRightWheel = 250;
      break;
    case 's': // slow straight-line forward drive
      desLeftWheel  = 40;
      desRightWheel = 40;
      break;
    case 'l': // turn left
      desLeftWheel  = 0;
      desRightWheel = -100;
      break;
    case 'r': // turn right
      desLeftWheel  = 100;
      desRightWheel = 0;
      break;
    case 'b': // slow straight-line back up 
      desLeftWheel  = -125;
      desRightWheel = -125;
      break;
    case 'p': // pause
      desLeftWheel  = 0;
      desRightWheel = 0;
      break;
    default: // default state is paused
      desLeftWheel  = 0;
      desRightWheel = 0;

  }

  // compute outputs based on updated measured and desired angular velocities
  leftWheelAngVelPID.Compute();
  rightWheelAngVelPID.Compute();

  // send motor outputs to slave controllers;
  giveMotorOutput(LEFT_WHEEL_ADDRESS, pwmLeftWheel);
  giveMotorOutput(RIGHT_WHEEL_ADDRESS, pwmRightWheel);

  if (millis() - heartBeatTime > 500) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    heartBeatTime = millis();
    Serial.print("Drive state: ");
    Serial.println(desiredOutputSwitch);
    //  Serial.print("Desired wheel velocity: ");
    //  Serial.println(desLeftWheel);
    Serial.print("Measured wheel velocity: ");
    Serial.println(measLeftWheel);
    Serial.print("PWM command: ");
    Serial.println(pwmLeftWheel);
  }

  // loop delay
  delay(50);
}

// provide updated PWM and direction outputs to slave controllers
void giveMotorOutput(int address, double output) {
  // local container to package outgoing payload
  byte outPayload[ENC_OUT_PAYLOAD_SIZE];

  // reconfigure output to be packaged into byte payload
  if (output < 0) {
    output = -output;
    outPayload[0] = 0; // 0 indicates reverse direction
  }
  else {
    outPayload[0] = 1;
  }
  outPayload[1] = (byte) output;

  // send payload to slave controller
  Wire.beginTransmission(address);
  Wire.write(outPayload, ENC_OUT_PAYLOAD_SIZE);
  Wire.endTransmission();
}

// request and process wheel angular velocity data from slave controllers
void getWheelAngVel(int address, double &wheelAngVel) {
  // local container to store incoming payload
  byte InPayload[ENC_IN_PAYLOAD_SIZE];

  // i2c comm to get payload from slave controller
  Wire.requestFrom(address, ENC_IN_PAYLOAD_SIZE);
  if (Wire.available() == ENC_IN_PAYLOAD_SIZE) {
    // get  wheel speed
    for (int i = 0; i < ENC_IN_PAYLOAD_SIZE; i++) {
      InPayload[i] = Wire.read();
    }
    wheelAngVel = (double) unpackWheelAngVel(InPayload);
  }
}

// process raw payload into wheel angular velocity
float unpackWheelAngVel(byte Data[5]) {
  // union structure to unpack byte array into float
  for  (int i = 0; i < ENC_IN_PAYLOAD_SIZE - 1; i++) {
    u.byteArray[i] = Data[i + 1];
  }
  float wheelAngVel = u.fval;

  // assign directionality
  if (Data[0] == 1) {
    wheelAngVel = -1 * wheelAngVel;
  }
  return wheelAngVel;
}

// double getDesiredWheelAngVel(int address) {
//   // local container to store incoming payload
//   byte InPayload[US_IN_PAYLOAD_SIZE];

//   // i2c comm to get payload from slave controller
//   Wire.requestFrom(address, ENC_IN_PAYLOAD_SIZE);
//   if (Wire.available() == ENC_IN_PAYLOAD_SIZE) {
//     // get wheel speed
//     for (int i = 0; i < ENC_IN_PAYLOAD_SIZE; i++) {
//       InPayload[i] = Wire.read();
//     }
//      wheelAngVel = unpackWheelAngVel(InPayload);
//   }
// }
