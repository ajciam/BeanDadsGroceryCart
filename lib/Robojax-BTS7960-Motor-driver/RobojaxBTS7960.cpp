/**
 * File: RobojaxBTS7960.cpp
written by Ahmad Shamshiri (Robojax.com) on Jun 22, 2019
 In Ajax, Ontario, Canada
 * Description:
 * RobojaxBTS7960 is an Arduino library for controlling BTS7960 motor driver chip
 *   
 * Author: Ahmad Shamshiri
 * Contact: Robojax.com
 * Copyright: 2019 Ahmad Shamshiri
 * Credits: Hans van Neck
 * Copying permission statement:
    This file is part of RobojaxBTS7960.

    LcdBarGraphX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Arduino.h"
#include "RobojaxBTS7960.h"


// -- constructor
RobojaxBTS7960::RobojaxBTS7960(int INH_R, int IN_R, int IS_R, int INH_L, int IN_L, int IS_L, int DEBUG)
{
    // -- pins defined
    _inh_r =INH_R;
	_in_r =IN_R;
	_is_r =IS_R;
	
    _inh_L =INH_L;
	_in_L =IN_L;
	_is_L =IS_L;
	
	_debug = DEBUG;


}

void RobojaxBTS7960::begin()
{
	pinMode(this->_inh_r, OUTPUT);
	pinMode(this->_in_r, OUTPUT);
	pinMode(this->_is_r, INPUT);	
	
	pinMode(this->_inh_L, OUTPUT);
	pinMode(this->_in_L, OUTPUT);
	pinMode(this->_is_L, INPUT);	

	if(this->_debug){
		Serial.println("BTS7960 Motor Control");
		Serial.print("Library version:");
		Serial.println(this->version);
		Serial.println("by Robojax.com");		
	}	

}

// -- the draw function
void RobojaxBTS7960::rotate(int value, int dir) {

    digitalWrite(this->_inh_r, HIGH);
	digitalWrite(this->_inh_L, HIGH);
	int pwm1Pin, pwm2Pin;
	if(dir ==1){
		pwm1Pin =this->_in_L;
		pwm2Pin =this->_in_r;
	}else{
		pwm1Pin =this->_in_r;
		pwm2Pin =this->_in_L;		
	}
	if(this->_debug){
		digitalWrite(pwm1Pin, LOW);
		Serial.print("PWM1: ");
		Serial.print(pwm1Pin);
		Serial.print(" PWM2: ");
		Serial.print(pwm2Pin);
		Serial.print(" PWM value: ");
		Serial.print(value);
		Serial.println("%");		
	}
     

    if(value >=0 && value <=100 ){
		
		analogWrite(pwm2Pin, toPWM(value));
	}

 
}//rotate

/*
*
*
*/
void RobojaxBTS7960::stop(){
    digitalWrite(this->_in_r, LOW);
	digitalWrite(this->_in_L, LOW);
	if(this->_debug){
		Serial.println("Motor stopped");
	}	
}//stop()

/*
 * @brief converts % value from 0 to 100% to 0-255
 * @param v is integer value representing % from 0-100
 * @return will return value from 0 to 255
 */
int RobojaxBTS7960::toPWM(int v){
  return map(v, 0,100,0,255);
}//

