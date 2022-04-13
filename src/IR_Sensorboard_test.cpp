#include <Arduino.h>

int IRSensor = 2; // connect ir sensor to arduino pin 2

void setup()
{
    Serial.begin(9600);
    pinMode(IRSensor, INPUT); // sensor pin INPUT
}

void loop()
{
    int statusSensor = digitalRead(IRSensor);
    Serial.print(statusSensor);
}