#include <Arduino.h>
#include <util/atomic.h> //For time
#include <Wire.h>        //i2c com
#include <math.h>
#include "BTS7960.h" //Motor Driver Libaray for the double H-Brdige BTS7960
#include "NewPing.h"
#include <Ultrasonic.h>

/*------------ CLASS ------------*/
class SimplePID
{
private:
  float kp, kd, ki, umax;
  float eprev, eintegral;

public:
  // Default initialization list
  SimplePID() : kp(1), kd(0), ki(0), umax(255), eprev(0.0), eintegral(0.0) {}

  // Set the parameters
  void setParams(float kpIn, float kdIn, float kiIn, float umaxIn)
  {
    kp = kpIn;
    kd = kdIn;
    ki = kiIn;
    umax = umaxIn;
  }

  // Evaluate the signal
  void evalu(int value, int target, float deltaT, int &pwr, int &dir)
  {

    // error
    int e = target - value;

    float dedt = (e - eprev) / (deltaT);
    eintegral = eintegral + e * deltaT;
    float u = kp * e + kd * dedt + ki * eintegral;

    // motor power
    pwr = (int)fabs(u);
    if (pwr > umax)
    {
      pwr = umax;
    }

    // motor direction
    dir = 1;
    if (u < 0)
    {
      dir = -1;
    }

    // store previous error
    eprev = e;
  }
};

/*------------ GLOBALS AND DEFINITIONS ------------*/
// Define the motors
#define NMOTORS 2
#define M0 0
#define M1 1
const int enca[] = {18, 2};
const int encb[] = {19, 3};
const int pwm_R[] = {5, 6};
const int pwm_L[] = {4, 7};
const int EN[] = {8, 8}; // Enable pins for motors
// Define the ultrasonic sensors
#define TRIGGER_PIN 51
#define ECHO_PIN 50
#define MAX_DISTANCE 400
#define TRIGGER_PIN_RIGHT 49
#define ECCH_PIN_RIGHT 48
#define MAX_DISTANCE_RIGHT 400
#define TRIGGER_PIN_MIDDLE 47
#define ECCH_PIN_MIDDLE 46
#define MAX_DISTANCE_MIDDLE 400

// IR Sensors
int rightIR = 44; // connect ir sensor out to pin 44
int leftIR = 45;
// Global variables
long prevT = 0;
int posPrev[] = {0, 0};
int distance = 0;
unsigned long previousMillis = 0;
const long printInterval = 1000; // print interval in milliseconds
// positions
volatile int posi[] = {0, 0};

// PID classes
SimplePID pid[NMOTORS];
// Ultrasonic ultrasonic1(TRIGGER_PIN, ECHO_PIN);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonarRight(TRIGGER_PIN_RIGHT,ECCH_PIN_RIGHT,MAX_DISTANCE_RIGHT);
NewPing sonarMiddle(TRIGGER_PIN_MIDDLE,ECCH_PIN_MIDDLE,MAX_DISTANCE_MIDDLE);
// Motor Classes
// BTS7960 motorController[NMOTORS]; // define motor 1 objects

/*------------ FUNCTIONS ------------*/

void setMotor(int dir, int pwmVal, int pwm_R, int pwm_L)
{
  int PWMPIN_LEFT = pwm_L;
  int PWMPIN_RIGHT = pwm_R;
  if (dir == -1)
  {
    analogWrite(PWMPIN_LEFT, 0);
    delayMicroseconds(100);
    analogWrite(PWMPIN_RIGHT, pwmVal);
  }
  else if (dir == 1)
  {
    analogWrite(PWMPIN_RIGHT, 0);
    delayMicroseconds(100);
    analogWrite(PWMPIN_LEFT, pwmVal);
  }
  else
  {
    analogWrite(PWMPIN_LEFT, LOW);
    analogWrite(PWMPIN_RIGHT, HIGH);
  }
}
template <int j>
void readEncoder()
{
  int b = digitalRead(encb[j]);
  if (b > 0)
  {
    posi[j]++;
  }
  else
  {
    posi[j]--;
  }
}

// void sendLong(long value){
//   for(int k=0; k<4; k++){
//     byte out = (value >> 8*(3-k)) & 0xFF;
//     Wire.write(out);
//   }
// }

// long receiveLong(){
//   long outValue;
//   for(int k=0; k<4; k++){
//     byte nextByte = Wire.read();
//     outValue = (outValue << 8) | nextByte;
//   }
//   return outValue;
// }

// targets
float target_f[] = {0, 0, 0, 0};
long target[] = {0, 0, 0, 0};

void setTarget(float t, float deltat, int distanceLeft,int distanceRight,int distanceMiddle, int rightIRStatus, int leftIRStatus)
{

  float positionChange[4] = {0.0, 0.0, 0.0, 0.0};
  float pulsesPerTurn = 16 * 30;                 // 16 pules per revolution * Gear Ratio
  float pulsesPerMeter = pulsesPerTurn * 2.0886; // Pulses per turn*(1/Wheel cirumfernce (m))

  t = fmod(t, 12);     // time is in seconds to repeated a movment cycle over 12 seconds
  float velocity = .3; // m/s

  // Logic for the Sensors here; probably better to seprate into seperate fuctnion, but with the way PID is implemented this was easier
  // More readble if done in Switch cases or State Machines but for now its fine
  if (((distanceLeft > 200)  || (distanceLeft > 16 && distanceLeft <= 30)) || ((distanceRight > 200)  
  || (distanceRight > 16 && distanceRight <= 30)) || ((distanceMiddle > 200)  
  || (distanceMiddle > 16 && distanceMiddle <= 30))) //STop conditons
  {
    for (int k = 0; k < 4; k++)
    {
      positionChange[k] = 0;
    }
  }
  else if ((distanceLeft > 20 && distanceLeft < 100) &&((distanceRight > 20 && distanceRight < 100)) && ((distanceMiddle > 20 && distanceMiddle < 100))) // FWD Condition
  {
    for (int k = 0; k < 4; k++)
    {
      positionChange[k] = velocity * deltat * pulsesPerMeter;
    }
  }
  else if ((distanceLeft < 15 && distanceLeft >2) || ((distanceRight < 15 && distanceRight >2)
  || (distanceMiddle < 15 && distanceMiddle >2)) )//&& rightIRStatus == 1 && leftIRStatus == 1) // Revesere condition
  { 
    velocity = .3;
    for (int k = 0; k < 4; k++)
    {
      positionChange[k] = -velocity * deltat * pulsesPerMeter;
    }
  }
  else if (rightIRStatus == 0) // Right Turn Condition
  {
    positionChange[0] = velocity * deltat * pulsesPerMeter; // Turn Left wheel
  }
  else if (leftIRStatus == 0) // Right Turn Condition
  {
    positionChange[1] = velocity * deltat * pulsesPerMeter; // Turn Right wheel
  }
  for (int k = 0; k < 2; k++)
  {
    target_f[k] = target_f[k] + positionChange[k];
  }
  target[0] = (long)target_f[0];
  target[1] = (long)-target_f[1];
}

/*------------ SETUP ------------*/
void setup()
{
  Serial.begin(9600);
  for (int k = 0; k < NMOTORS; k++)
  {
    pinMode(EN[k], OUTPUT);
    digitalWrite(EN[k], HIGH); // Enable motor Driver
    pinMode(enca[k], INPUT);
    pinMode(encb[k], INPUT);
    pid[k].setParams(1.5, .2, 0, 255);
  }
  attachInterrupt(digitalPinToInterrupt(enca[M0]), readEncoder<M0>, RISING);
  attachInterrupt(digitalPinToInterrupt(enca[M1]), readEncoder<M1>, RISING);
  pinMode(rightIR, INPUT); // Right IR sensor pin INPUT
  pinMode(leftIR, INPUT);  // Right IR sensor pin INPUT
  Serial.println("End of setup");
}
/*------------ LOOP ------------*/
void loop()
{

  // time difference
  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / (1.0e6);
  prevT = currT;
  int distanceLeft = sonar.ping_cm(); // Distnace Calcuaiton for US sensor
  int distanceRight = sonarRight.ping_cm(); //Distance for front right ultrasonic sensor
  int distanceMiddle = sonarMiddle.ping_cm(); //Distance for front middle ultrasonic sensor

  //.int distnace = ultrasonic1.read(); //Distnace w/ ultrasonic lib.
  int statusIR[] = {digitalRead(rightIR), digitalRead(leftIR)}; // Read IR Sensors and puty

  // set target position
  setTarget(currT / 1.0e6, deltaT, distanceLeft,distanceRight,distanceMiddle, statusIR[0], statusIR[1]);

  // Send the requested position to the follower

  // Get the current position from the follower

  // Read the position in an atomic block to avoid a potential misread  (Encoder)
  int pos[] = {0, 0};
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    for (int k = 0; k < NMOTORS; k++)
    {
      pos[k] = posi[k];
    }
  }

  // Loop through the motors
  for (int k = 0; k < NMOTORS; k++)
  {
    int pwr, dir;
    pid[k].evalu(pos[k], target[k], deltaT, pwr, dir); // compute the position
    // setMotor(dir,pwr,pwm[k],in1[k],in2[k]); // signal the motor
    setMotor(dir, pwr, pwm_R[k], pwm_L[k]); // signal the motor
  }

  // for(int i = 0; i<2; i++){
  //   Serial.print(target[i]);
  //   Serial.print(" ");
  // }
  // for(int i = 0; i<2; i++){
  //   Serial.print(pos[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();

  // Send results to Serial Monitor
  unsigned long currentMillis = millis(); // all time elements are data type unsigned long

  if (currentMillis - previousMillis >= printInterval)
  { // enough time passed yet?
    previousMillis = currentMillis;
    Serial.println("Sensor Right: ");
    Serial.println(distanceRight); // Prints the distance on the default unit (centimeters)
    Serial.print("cm");
    Serial.println("Sensor Left:");
    Serial.println(distanceLeft); // Prints the distance on the default unit (centimeters)
    Serial.print("cm");
    Serial.println("Sensor Middle");
    Serial.println(distanceMiddle); // Prints the distance on the default unit (centimeters)
    Serial.print("cm");


  }
  // setMotor(-1,50,pwm_R[0],pwm_L[0]);
}
