#include <Arduino.h>
#include <util/atomic.h>    // For the ATOMIC_BLOCK macro
#include <RobojaxBTS7960.h> //Motor Driver Lib.
#include "BTS7960.h"        //Motor Driver Lib. 2 first one has bugs

#define ENCA 18 // Yellow Encoder Pinout: Gray END GGround, Blue Encoder VCC (3.3v to 24v)
#define ENCB 19 // White

#define EN 8
#define L_PWM 3
#define R_PWM 2

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
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
motorController1.TurnRight(85);    //Runs at around 50% power Motor runs at 330 rpm no load from data
delay(5000);
}
  
 
