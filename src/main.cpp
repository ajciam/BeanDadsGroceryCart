#include <Arduino.h>
#include <RobojaxBTS7960.h>
/*
Main Code for robot Movement, and soon sensors. Sensors would be contained in different cpp files and 
*/


// pins for motor 1
#define RPWM_1 2 // define pin 3 for RPWM pin (output)
#define R_EN_1 8 // define pin 2 for R_EN pin (input)
#define R_IS_1 52 // define pin 5 for R_IS pin (output)

#define LPWM_1 3 // define pin 6 for LPWM pin (output)
#define L_EN_1 8 // define pin 7 for L_EN pin (input)
#define L_IS_1 50 // define pin 8 for L_IS pin (output)
// motor 1 pins end here

// pins for motor 2
#define RPWM_2 9 // define pin 9 for RPWM pin (output)
#define R_EN_2 10 // define pin 10 for R_EN pin (input)
#define R_IS_2 12 // define pin 12 for R_IS pin (output)

#define LPWM_2 11 // define pin 11 for LPWM pin (output)
#define L_EN_2 A0 // define pin 7 for L_EN pin (input)
#define L_IS_2 A1 // define pin 8 for L_IS pin (output)
// motor 2 pins end here



#define CW 1 //
#define CCW 0 //
#define debug 1 //

RobojaxBTS7960 motor1(R_EN_1,RPWM_1,R_IS_1, L_EN_1,LPWM_1,L_IS_1,debug);//define motor 1 object
RobojaxBTS7960 motor2(R_EN_2,RPWM_2,R_IS_2, L_EN_2,LPWM_2,L_IS_2,debug);//define motor 2 object and the same way for other motors

void setup() {
  // BTS7960 Motor Control Code by Robojax.com 20190622
  Serial.begin(9600);// setup Serial Monitor to display information

   motor1.begin();
   motor2.begin();   
   
    // BTS7960 Motor Control Code by Robojax.com 20190622 
}

void loop() {
   // BTS7960 Motor Control Code by Robojax.com 20190622 
    motor1.rotate(100,CW);// run motor 1 with 100% speed in CW direction
    delay(5000);//run for 5 seconds
    motor1.stop();// stop the motor 1
    delay(3000);// stop for 3 seconds
    motor1.rotate(100,CCW);// run motor 1 at 100% speed in CCW direction
    delay(5000);// run for 5 seconds
    motor1.stop();// stop the motor 1
    delay(3000);  // stop for 3 seconds

    // motor2.rotate(100,CW);// run motor 2 with 100% speed in CW direction
    // delay(5000);//run for 5 seconds
    // motor2.stop();// stop the motor 2
    // delay(3000);// stop for 3 seconds
    // motor2.rotate(100,CCW);// run motor 2 at 100% speed in CCW direction
    // delay(5000);// run for 5 seconds
    // motor2.stop();// stop the motor 2
    // delay(3000);  // stop for 3 seconds

    
  // slowly speed up the motor 1 from 0 to 100% speed
    // for(int i=0; i<=100; i++){ 
    //     motor1.rotate(i,CCW);
    //     delay(50);
    // } 
  
  //  // slow down the motor  2 from 100% to 0 with 
  //   for(int i=100; i>0; i--){ 
  //       motor2.rotate(i,CCW);
  //       delay(50);
  //   } 
//     motor2.stop();// stop motor 2
//     delay(3000); // stop for 3 seconds        
//  // BTS7960 more than 1 Motor Control Code by Robojax.com 20190622  
 }// loop ends
