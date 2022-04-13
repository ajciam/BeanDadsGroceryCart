#include <Arduino.h>
#include <util/atomic.h>    // For the ATOMIC_BLOCK macro
#include <RobojaxBTS7960.h> //Motor Driver Lib.
#include "BTS7960.h"        //Motor Driver Lib. 2 first one has bugs

#define ENCA_1 18 // Yellow Encoder Pinout: Gray END GGround, Blue Encoder VCC (3.3v to 24v)
#define ENCB_1 19 // White

#define EN_1 8
#define L_PWM_1 3
#define R_PWM_1 2

#define EN_2 9
#define L_PWM_2 5
#define R_PWM_2 4

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
// PID Variables
long prevT = 0;
float eprev = 0;
float eintegral = 0;

// Fucntions: setMotor and readEncoder

// Object declaration
BTS7960 motorController1(EN_1, L_PWM_1, R_PWM_1); // define motor 1 object
BTS7960 motorController2(EN_2, L_PWM_2, R_PWM_2); // define motor 1 object

void readEncoder()
{
  int b = digitalRead(ENCB_1);
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

  pinMode(ENCA_1, INPUT); // Encoder Pin setup
  pinMode(ENCB_1, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_1), readEncoder, RISING);

  Serial.println("target pos");
  motorController1.Enable();
  motorController2.Enable();
}

// Main Loop
void loop()
{
motorController2.TurnRight(255);    //0-255 pmw, 85=30% 230=90%
motorController1.TurnRight(255);  
delay(5000);
}