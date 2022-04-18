#include <Arduino.h>
#include <util/atomic.h>    // For the ATOMIC_BLOCK macro
#include <RobojaxBTS7960.h> //Motor Driver Lib.
#include "BTS7960.h"        //Motor Driver Lib. 2 first one has bugs

#define ENCA 18 // Yellow Encoder Pinout: Gray END GGround, Blue Encoder VCC (3.3v to 24v)
#define ENCB 19 // White

#define EN 8
#define L_PWM 4
#define R_PWM 5

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/

//Max speed PWM signal
double MAX_SPEED=255;
// PID Variables
long prevT = 0;
float eprev = 0;
float eintegral = 0;


// Fucntions: setMotor and readEncoder

// Object declaration
BTS7960 motorController1(EN, L_PWM, R_PWM); // define motor 1 object

void readEncoder()
{
  int b = digitalRead(ENCB);
  if (b > 0)
  {
    posi++;
  }
  else
  {
    posi--;
  }
}




// Setup
void setup()
{
  // BTS7960 Motor Control Code by Robojax.com 20190622
  Serial.begin(9600); // setup Serial Monitor to display information

  pinMode(ENCA, INPUT); // Encoder Pin setup
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

  Serial.println("target pos");
  motorController1.Enable();
}

// Main Loop
void loop()
{
  // set target position
  int target = 120000;
  //int target = 1250 * sin(prevT / 1e6);

  // PID constants
  float kp = 0;
  float kd = 0;
  float ki = 0.0;

  // time difference
  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / (1.0e6);
  prevT = currT;

  // Read the position in an atomic block to avoid a potential
  // misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    pos = posi;
  }

  // error
  int e = pos - target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr = fabs(u);
  if( pwr > MAX_SPEED ){
    pwr = MAX_SPEED;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  if(dir==1){
    motorController1.TurnRight(pwr);
  }
  else if(dir==-1){
    motorController1.TurnLeft(pwr);
  }
  else{

  }

  // store previous error
  eprev = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}
  
 
