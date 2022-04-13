#include <Wire.h>
#include <PID_v1.h>

// i2c communication
#define IN_PAYLOAD_SIZE 5     // incoming bytes from slaves
#define OUT_PAYLOAD_SIZE 2    // outgoing bytes to slaves
#define LEFT_WHEEL_ADDRESS 2  // address of left wheel  
#define RIGHT_WHEEL_ADDRESS 3 // address of right wheel

// containers
char desiredOutputSwitch = 'f';

// PID
double  measLeftWheel, desLeftWheel, pwmLeftWheel;
double leftKp = 1;
double leftKi = .01;
double leftKd = .5;
double measRightWheel, desRightWheel, pwmRightWheel;
double rightKp = 1;
double rightKi = .01;
double rightKd = .5;
PID leftwheelAngVelPID(&measLeftWheel, &pwmLeftWheel, &desLeftWheel, leftKp, leftKi, leftKd, DIRECT);
PID rightwheelAngVelPID(&measRightWheel, &pwmRightWheel, &desRightWheel, rightKp, rightKi, rightKd, DIRECT);

void setup() {
  Serial.begin(9600);

  // set limits on the PID outputs
  leftwheelAngVelPID.SetOutputLimits(-255,255);
  rightwheelAngVelPID.SetOutputLimits(-255,255);

  // start I2C
  Wire.begin();

  leftwheelAngVelPID.SetMode(AUTOMATIC);
}

void loop() {

  if (Serial.available() > 0) {
    desiredOutputSwitch = Serial.read();
  }
  Serial.print("Drive state: ");
  Serial.println(desiredOutputSwitch);
  Serial.print("PWM: ");
  Serial.println(pwmLeftWheel);
  Serial.print("Desired wheel velocity: ");
  Serial.println(desLeftWheel);
  Serial.print("Measured wheel velocity: ");
  Serial.println(measLeftWheel);

  
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
      desLeftWheel  = 125;
      desRightWheel = 125;
    case 'l': // turn left
      desLeftWheel  = 0;
      desRightWheel = 100;
      break;
    case 'r': // turn right
      desLeftWheel  = 100;
      desRightWheel = 0;
      break;
    case 'b': // slow straight-line back up 
      desLeftWheel  = -125;
      desRightWheel = -125;
    case 'p': // pause
      desLeftWheel  = 0;
      desRightWheel = 0;
      break;
    default: // default state is paused
      desRightWheel = 250;
      desLeftWheel  = 250;
  }

  // compute outputs based on updated measured and desired angular velocities
  leftwheelAngVelPID.Compute();
  rightwheelAngVelPID.Compute();

  // send motor outputs to slave controllers;
  giveMotorOutput(LEFT_WHEEL_ADDRESS, pwmLeftWheel);
  giveMotorOutput(RIGHT_WHEEL_ADDRESS, pwmRightWheel);

  // loop delay
  delay(500);
}

// request and process wheel angular velocity data from slave controllers
void getWheelAngVel(byte address, double &wheelAngVel) {
  // local container to store incoming payload
  byte InPayload[IN_PAYLOAD_SIZE];

  // i2c comm to get payload from slave controller
  Wire.requestFrom(address, IN_PAYLOAD_SIZE);
  if (Wire.available() == IN_PAYLOAD_SIZE) {
    // get  wheel speed
    for (int i = 0; i < IN_PAYLOAD_SIZE; i++) {
      InPayload[i] = Wire.read();
    }
    wheelAngVel = unpackWheelAngVel(InPayload);
  }
}

// provide updated PWM and direction outputs to slave controllers
void giveMotorOutput(byte address, double output) {
  // local container to package outgoing payload
  byte outPayload[OUT_PAYLOAD_SIZE];

  // reconfigure output to be packaged into byte payload
  if (output < 0) {
    output = -output;
    outPayload[0] = 0; // 0 indicates reverse direction
  }
  else {
    outPayload[0] = 1;
  }
  outPayload[1] = (byte) output; // check if this type conversion works

  // send payload to slave controller
  Wire.beginTransmission(address);
  Wire.write(outPayload, OUT_PAYLOAD_SIZE);
  Wire.endTransmission();
}

// process raw payload into wheel angular velocity
double unpackWheelAngVel(byte Data[5]) {
  // union structure to unpack byte array into float
  union float2byteArray {
      byte byteArray[4];
      float fval;
  } u;
  u.byteArray[0] = Data[1];
  u.byteArray[1] = Data[2];
  u.byteArray[2] = Data[3];
  u.byteArray[3] = Data[4];
  float wheelAngVel = u.fval;

  // assign directionality
  if (Data[0] = 0) {
      wheelAngVel = -wheelAngVel;
  }
  return (double) wheelAngVel;
}

