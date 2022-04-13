#include <NewPing.h>
#include <Wire.h>
#include <Servo.h>

#define MAX_DISTANCE 250
#define OUT_PAYLOAD_SIZE 1
#define ADDRESS 4

// pin initialization
const byte frontTrigPin      = 
const byte frontEchoPin      = 
const byte frontLeftTrigPin  = 
const byte frontLeftEchoPin  = 
const byte frontRightTrigPin = 
const byte frontRightEchoPin = 
const byte leftTrigPin       = 
const byte leftEchoPin       = 
const byte rightTrigPin      = 
const byte rightEchoPin      =    

// constants
const int forwardThresh = 200; // cm


// array of ultrasonic sensor objects
NewPing sonar[SONAR_NUM] = {
  NewPing(frontTrigPin,frontEchoPin, MAX_DISTANCE),
  NewPing(frontLeftTrigPin,frontLeftEchoPin, MAX_DISTANCE),
  NewPing(frontRightTrigPin, frontRightEchoPin, MAX_DISTANCE)
  NewPing(leftTrigPin,leftEchoPin, MAX_DISTANCE), 
  NewPing(rightTrigPin, rightEchoPin, MAX_DISTANCE)
};

int Dist[3];
byte Packet[2];

void setup() {
    Wire.begin(ADDRESS);
    Wire.onRequest(requestEvent);
}

void loop() {
    for (uint8_t i = 0; i < SONAR_NUM; i++) { // loop through each sensor and display results
        delay(50); // wait 50 ms between pings
        Dist[i] = sonar[i].ping_cm(); // 0 = outside distance range
    }
    if ((Dist[0] < forwardThresh) && (Dist[0] != 0))
        // check for most open side  and then command the system to rotate towards it
        if (((Dist[4] >=  Dist[5]) && (Dist[5] != 0)) || (Dist[4] == 0)) {
            // payload package to turn to face the left
            
        }
        else {
            // payload package to turn and face the right
        }
    else if ((Dist[1] < forwardThresh) && (Dist[1] != 0)) {
        // payload package to turn and face the right 
    }
    else if ((Dist[2] < forwardThresh) && (Dist[2] != 0)) {
        // payload package to turn and face the left
    }
    else {
        // payload package to drive forward
    }
}

void requestEvent() {
    Wire.write(Dist, OUT_PAYLOAD_SIZE);
}