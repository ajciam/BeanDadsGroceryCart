/* Robot_SimpleMotor_Drive_V0 with encoders
 * 
 * Adafruit Feather M4 using Pololu TB6612FNG motor controller
 * Drives two motors at fixed speed with PI control
 * 
 * Motor Control Table
 * XIN1   XIN2    Effect
 * Low    Low     Brake
 * Low    High    Forward
 * High   Low     Reverse
 * 
 * Free to use for all
 * F Milburn, January 2020
 */
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
#define L_PWM_2 4 //Red
#define R_PWM_2 5 //Brown
//Motor Objects

// Object declaration
BTS7960 motorController1(EN_1, L_PWM_1, R_PWM_1); // define motor 1 object
BTS7960 motorController2(EN_2, L_PWM_2, R_PWM_2); // define motor 1 object
// PWM
const uint16_t ANALOG_WRITE_BITS = 8;
const uint16_t MAX_PWM = pow(2, ANALOG_WRITE_BITS)-1;
const uint16_t MIN_PWM = MAX_PWM / 4;    // Make sure motor turns
// Motor timing
unsigned long nowTime = 0;       // updated on every loop
unsigned long startTimeA = 0;    // start timing A interrupts
unsigned long startTimeB = 0;    // start timing B interrupts
unsigned long countIntA = 0;     // count the A interrupts
unsigned long countIntB = 0;     // count the B interrupts
double periodA = 0;              // motor A period
double periodB = 0;              // motor B period
//Encoder
double pos1 = 0;
double pos2 = 0;
// PID 
const unsigned long SAMPLE_TIME = 100;  // time between PID updates
const unsigned long INT_COUNT = 20;     // sufficient interrupts for accurate timing
double setpointA = 150;         // setpoint is rotational speed in Hz
double inputA = 0;              // input is PWM to motors
double outputA = 0;             // output is rotational speed in Hz
double setpointB = 150;         // setpoint is rotational speed in Hz
double inputB = 0;              // input is PWM to motors
double outputB = 0;             // output is rotational speed in Hz
double KpA = 0.20, KiA = 0.20, KdA = 0;
double KpB = 0.20, KiB = 0.20, KdB = 0;
PID motorA(&inputA, &outputA, &setpointA, KpA, KiA, KdA, DIRECT);
PID motorB(&inputB, &outputB, &setpointB, KpB, KiB, KdB, DIRECT);
double storeB = 0;               // used for debug print


/* Functions */

void readEncoder1(){
  int b = digitalRead(ENCB_1);
  if(b > 0){
    pos1++;
  }
  else{
    pos1--;
  }
}
void readEncoder2(){
  int b = digitalRead(ENCB_2);
  if(b > 0){
    pos2++;
  }
  else{
    pos2--;
  }
}
void isr_A(){
  // count sufficient interrupts to get accurate timing
  // inputX is the encoder frequency in Hz
  countIntA++;
  if (countIntA == INT_COUNT){
    inputA = (float) INT_COUNT * 1000 / (float)(nowTime - startTimeA);
    startTimeA = nowTime;
    countIntA = 0;
  }
}
void isr_B(){
  // count sufficient interrupts to get accurate timing
  // inputX is the encoder frequency in Hz
  countIntB++;
  if (countIntB == INT_COUNT){
    inputB = (float) INT_COUNT * 1000 / (float)(nowTime - startTimeB);
    startTimeB = nowTime;
    countIntB = 0;
  }
}

void initEncoders(){
  pinMode(ENCA_1, INPUT);
  pinMode(ENCB_1, INPUT);
  pinMode(ENCA_2, INPUT);
  pinMode(ENCB_2, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_1), readEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_2), readEncoder2, RISING);
  
}
void initPWM(){
  startTimeA = millis();
  startTimeB = millis();
  motorA.SetOutputLimits(MIN_PWM, MAX_PWM);
  motorB.SetOutputLimits(MIN_PWM, MAX_PWM);
  motorA.SetSampleTime(SAMPLE_TIME);
  motorB.SetSampleTime(SAMPLE_TIME);
  motorA.SetMode(AUTOMATIC);
  motorB.SetMode(AUTOMATIC);
}

void setup(){
Serial.begin(9600); // setup Serial Monitor to display information

Serial.println("Setup");
// turn on motors
motorController1.Enable();
motorController2.Enable();


}
void loop(){

motorController2.TurnRight(85);
motorController1.TurnLeft(85);
delay(1000);
Serial.print(pos1);
Serial.print(pos2);
  

  if (storeB != outputB){
    storeB = outputB;
    Serial.println("inputA, inputB, errorA, errorB");
    Serial.print(inputA); Serial.print("  ");
    Serial.print(inputB); Serial.print("  ");
    Serial.print(100*(setpointA-inputA)/setpointA); Serial.print("  ");
    Serial.print(100*(setpointB-inputB)/setpointB); Serial.println("\n");
  }
}
