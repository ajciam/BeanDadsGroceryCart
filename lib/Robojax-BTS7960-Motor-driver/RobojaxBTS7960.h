/**
 * File: RobojaxBTS7960.h
 
written by Ahmad Shamshiri (Robojax.com) on June 22, 2019
 In Ajax, Ontario, Canada
 * Description:
 * RobojaxBTS7960B is an Arduino library for controlling BTS7960B motor driver chip
 *   
 * Author: Ahmad Shamshiri
 * Contact: Robojax.com
 * Copyright: 2019 Ahmad Shamshiri
 * Credits: Hans van Neck
 * Copying permission statement:
    This file is part of RobojaxBTS7960.

   RobojaxBTS7970B is free software: you can redistribute it and/or modify
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

#ifndef RobojaxBTS7960_H
#define RobojaxBTS7960_H


#include "Arduino.h"


class RobojaxBTS7960
{
public:
    /**
    Get motor pin values
     */
    RobojaxBTS7960(int INH_R, int IN_R, int IS_R, int INH_L, int IN_L, int IS_L, int DEBUG);
    /**
     * control motor
     */
    void rotate(int value, int dir);
   /**
    * Initializes the display.
    */
   void begin();
   
   /*
   * stops the motor
   
   */
   void stop();   

	
private:

    int toPWM(int v);
    int _inh_r;
	int _in_r;
	int _is_r;
    int _inh_L;
	int _in_L;
	int _is_L;
	int _debug;
	double version = 1.00;


};

#endif
