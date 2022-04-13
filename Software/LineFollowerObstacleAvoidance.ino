//Arduino Line Follower Obstacle Avoidance Robot

/*You have to install the NewPing library before uploading the code
  Follow this procedure to install the NewPing library First download the NewPing.zip library >> click on sketch tab >> Include Library
  >> Add.ZIP LIBRARY >> Select NewPing.zip file >> Done
  
  Created By DIY Builder
*/

// NewPing Library Link NewPing library Link https://github.com/eliteio/Arduino_New_Ping //
// You can Modify this code according to your need//

#include<NewPing.h>

int ENA = 3;  //ENA connected to digital pin 3
int ENB = 9;  //ENB connected to digital pin 9
int MOTOR_A1 = 4; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 5; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 6; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 7; // MOTOR_B2 connected to digital pin 7

int RIGHT = A0; // RIGHT sensor connected to analog pin A0
int LEFT = A1;  // LEFT sensor connected to analog pin A0

#define TRIG A2 // TRIG PIN connected to analog pin A2
#define ECHO A3 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 100 // Define Maximum Distance

NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 

void setup() {
  // put your setup code here, to run once:
  
pinMode(ENA, OUTPUT); // initialize ENA pin as an output
pinMode(ENB, OUTPUT); // initialize ENB pin as an output
pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
pinMode(LEFT, INPUT);  // initialize LEFT pin as an input


}

void loop() {
  // put your main code here, to run repeatedly:
  
delay(70);
int distance = sonar.ping_cm();
if (distance == 0) {
  distance = 30;
}
if(distance <=15) {
  Stop();
  delay(100);
  turnRight();
  delay(350);
  moveForward();
  delay(500);
  turnLeft();
  delay(350);
  moveForward();
  delay(700);
  turnLeft();
  delay(300);
  moveForward();
  delay(400);
  turnRight();
  delay(400);
}
if (analogRead(RIGHT)<=35 && analogRead(LEFT)<=35) {
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  
}else if (analogRead(RIGHT)<=35 && !analogRead(LEFT)<=35) {
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);

}else if (!analogRead(RIGHT)<=35 && analogRead(LEFT)<=35) {
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  
}else if (!analogRead(RIGHT)<=35 && !analogRead(LEFT)<=35) {
 
  Stop();
}

}
void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  
}

void turnLeft() {
  analogWrite(ENA,100);
  analogWrite(ENB, 100);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

void moveForward() {
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

/*void turnAround() {
  
  
  
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);

  for (int fadeValue = 70; fadeValue <=200; fadeValue +=10) {
    analogWrite(ENA, fadeValue);
    digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  }

   for (int fadeValue = 200; fadeValue >=70; fadeValue -=10) {
    analogWrite(ENB, fadeValue);
    digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  }
}*/
