/*
* This is the Arduino code BTS7960 DC motor Driver 
Using this code you can control a motor to rotate in both direction Clockwise(CW) 
and Counter-clockwise(CCW)
 
// Written by Ahmad Shamshiri for Robojax.com on 
// on June 22, 2019 at 14:08 in Ajax, Ontario, Canada

If you found this tutorial helpful, please support me so I can continue creating content like this. You can support me on Patreon (
http://bit.ly/2ZnITLz

or make donation using PayPal http://bit.ly/rj-paypal

* BTS7960B
 * Code is available at http://robojax.com/learn/arduino

 * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. 

*/

// 
#define RPWM 3 // define pin 3 for RPWM pin (output)
#define R_EN 4 // define pin 2 for R_EN pin (input)
#define R_IS 5 // define pin 5 for R_IS pin (output)

#define LPWM 6 // define pin 6 for LPWM pin (output)
#define L_EN 7 // define pin 7 for L_EN pin (input)
#define L_IS 8 // define pin 8 for L_IS pin (output)
#define CW 1 //do not change
#define CCW 0 //do not change
#define debug 1 //change to 0 to hide serial monitor debugging infornmation or set to 1 to view

#include <RobojaxBTS7960.h>
RobojaxBTS7960 motor(R_EN,RPWM,R_IS, L_EN,LPWM,L_IS,debug);

void setup() {
  // BTS7960 Motor Control Code by Robojax.com 20190622
  Serial.begin(9600);// setup Serial Monitor to display information

   motor.begin();
   
    // BTS7960 Motor Control Code by Robojax.com 20190622 
}

void loop() {
   // BTS7960 Motor Control Code by Robojax.com 20190622 
    motor.rotate(100,CW);// run motor with 100% speed in CW direction
    delay(5000);//run for 5 seconds
    motor.stop();// stop the motor
    delay(3000);// stop for 3 seconds
    motor.rotate(100,CCW);// run motor at 100% speed in CCW direction
    delay(5000);// run for 5 seconds
    motor.stop();// stop the motor
    delay(3000);  // stop for 3 seconds
	// slowly speed up the motor from 0 to 100% speed
    for(int i=0; i<=100; i++){ 
        motor.rotate(i,CCW);
        delay(50);
    } 
	
   // slow down the motor from 100% to 0 with 
    for(int i=100; i>0; i--){ 
        motor.rotate(i,CCW);
        delay(50);
    } 
    motor.stop();// stop motor
    delay(3000); // stop for 3 seconds        
 // BTS7960 Motor Control Code by Robojax.com 20190622  
}// loop ends


