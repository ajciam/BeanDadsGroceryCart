#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

// pin definition
const int encoderAPin = 3;
const int encoderBPin = 2;
const int motorDirPin = 4;
const int motorPWMPin = 5;

//  parameters for velocity calculations
int clicksPerOutRev = 1920; // from Pololu specifications
long sampTime       = 500;  // number of milliseconds to count clicks

// containers
byte pwmWheel   = 0;
float wheelRPM  = 0;
long lastMillis = 0;

// define objects
Encoder enc(encoderAPin, encoderBPin);

void setup() {
    // initialize pins
    pinMode(motorDirPin, OUTPUT);
    pinMode(motorPWMPin, OUTPUT);

    Serial.begin(9600);
    Serial.println("Reading shaft RPM: ");

    // reset time, encoder
    lastMillis = millis();
    enc.write(0);
}

void loop() {
    // write outputs to motor
    digitalWrite(motorDirPin, 1);
    analogWrite(motorPWMPin, 100);

    if ((millis() - lastMillis) > sampTime) {
        // calculate wheel linear velocity (neglecting slip)
        wheelRPM = enc.read() * 60000.00 / (clicksPerOutRev * (millis() - lastMillis));
        // reset time, encoder
        lastMillis = millis();
        enc.write(0);
        Serial.println(wheelRPM);
    }
}
