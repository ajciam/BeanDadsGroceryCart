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


// Object declaration
BTS7960 motorController1(EN_1, L_PWM_1, R_PWM_1); // define motor 1 object
BTS7960 motorController2(EN_2, L_PWM_2, R_PWM_2); // define motor 1 object

// PID Objects for Motors
// PID motorA(&inputA, &outputA, &setpointA, KpA, KiA, KdA, DIRECT);
// PID motorB(&inputB, &outputB, &setpointB, KpB, KiB, KdB, DIRECT);
/* Functions */


// Setup
void setup()
{
     
// BTS7960 Motor Control Code by Robojax.com 20190622
Serial.begin(9600); // setup Serial Monitor to display information

Serial.println("target pos");
// turn on motors
motorController1.Enable();
motorController2.Enable();
// Turn on PID

 // Serial.println("target pos");
}

// Main Loop
void loop()
{
//motorController1.TurnRight(85);    //Runs at around 50% power Motor runs at 330 rpm no load from data
motorController2.TurnRight(85);
motorController1.TurnLeft(85);

delay(5000);
Serial.print("Debug");
//   motorController2.TurnRight(50); // 100 % = 330 rpm (0-255)
//   delay(5000);
}
=======
motorController2.TurnRight(255);    //0-255 pmw, 85=30% 230=90%
motorController1.TurnRight(255);  
delay(5000);
}

