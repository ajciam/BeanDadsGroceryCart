#include <Arduino.h>
#include <util/atomic.h>    // For the ATOMIC_BLOCK macro
#include <RobojaxBTS7960.h> //Motor Driver Lib.
#include "BTS7960.h"        //Motor Driver Lib. 2 first one has bugs
#include <PID_v1.h>         //PID Libarary
// Left Motor Encoder Pins
#define ENCA_1 18 // Yellow Encoder Pinout: Gray END GGround, Blue Encoder VCC (3.3v to 24v)
#define ENCB_1 19 // White

// Right Motor Encoder Pins
#define ENCA_2 14 // Yellow Encoder Pinout: Gray END GGround, Blue Encoder VCC (3.3v to 24v)
#define ENCB_2 15 // White
// Left Motor
#define EN_1 21
#define L_PWM_1 3
#define R_PWM_1 2
// Right Motor
#define EN_2 8
#define L_PWM_2 4
#define R_PWM_2 5

<<<<<<< HEAD
=======
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
>>>>>>> 1e0f5f712f08d072e1c026661c2b35de5694bda9

// Object declaration
BTS7960 motorController1(EN_1, L_PWM_1, R_PWM_1); // define motor 1 object
BTS7960 motorController2(EN_2, L_PWM_2, R_PWM_2); // define motor 1 object
<<<<<<< HEAD

// PID Objects for Motors
// PID motorA(&inputA, &outputA, &setpointA, KpA, KiA, KdA, DIRECT);
// PID motorB(&inputB, &outputB, &setpointB, KpB, KiB, KdB, DIRECT);
/* Functions */

=======

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
>>>>>>> 1e0f5f712f08d072e1c026661c2b35de5694bda9

// Setup
void setup()
{
     
// BTS7960 Motor Control Code by Robojax.com 20190622
Serial.begin(9600); // setup Serial Monitor to display information

<<<<<<< HEAD
Serial.println("target pos");
// turn on motors
motorController1.Enable();
motorController2.Enable();
// Turn on PID

 // Serial.println("target pos");
=======
  pinMode(ENCA_1, INPUT); // Encoder Pin setup
  pinMode(ENCB_1, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_1), readEncoder, RISING);

  Serial.println("target pos");
  motorController1.Enable();
  motorController2.Enable();
>>>>>>> 1e0f5f712f08d072e1c026661c2b35de5694bda9
}

// Main Loop
void loop()
{
<<<<<<< HEAD
//motorController1.TurnRight(85);    //Runs at around 50% power Motor runs at 330 rpm no load from data
motorController2.TurnRight(85);
motorController1.TurnLeft(85);

delay(5000);
Serial.print("Debug");
//   motorController2.TurnRight(50); // 100 % = 330 rpm (0-255)
//   delay(5000);
}
=======
<<<<<<< HEAD
motorController2.TurnRight(255);    //0-255 pmw, 85=30% 230=90%
motorController1.TurnRight(255);  
=======
motorController1.TurnRight(85);    //Runs at around 50% power Motor runs at 330 rpm no load from data
>>>>>>> 2ff14881332f9d5cf3b8d2ce4fb45c6e26d4c480
delay(5000);
}
>>>>>>> 1e0f5f712f08d072e1c026661c2b35de5694bda9
