/*
  Cayenne Digital Motion Sensor Example

  This sketch shows how to automatically send data to a Digital Motion Sensor in the Cayenne Dashboard.

  The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

  Steps:
  1. In the Cayenne Dashboard add a new Digital Motion Sensor widget.
  2. Set the widget to 2-State Display.
  3. Select Virtual Pins and a virtual pin number.
  4. Set VIRTUAL_PIN to the pin number you selected.
  5. Attach a Digital Motion Sensor to a digital pin on your Arduino.
   Schematic:
   [Ground] -- [Digital Motion Sensor] -- [5V]
                         |
                     Digital Pin
  6. Set the motionSensorPin1 variable to match the pin used to connect the motion sensor.
  7. Set the token variable to match the Arduino token from the Dashboard.
  8. Compile and upload this sketch.
  9. Once the Arduino connects to the Dashboard it should automatically update the Digital Motion Sensor widget with data.
*/
#include <CayenneSerial.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "h33ep7g1bu";

// Virtual Pin of the Digital Motion Sensor widget.
#define VIRTUAL_PIN1 V1
#define VIRTUAL_PIN2 V2
#define VIRTUAL_PIN3 V3
#define VIRTUAL_PIN4 V4
#define VIRTUAL_PIN5 V5



// Digital pin the motion sensor is connected to. Do not use digital pins 0 or 1 since those conflict with the use of Serial.
const int motionSensorPin1 = 12;
const int motionSensorPin2 = 11;
const int motionSensorPin3 = 10;
const int motionSensorPin4 = 9;
const int motionSensorPin5 = 8;
void setup()
{
  Cayenne.begin(token);
}

void loop()
{
  Cayenne.run();
  checkSensor();
}

int previousState1 = -1;
int previousState2 = -1;
int previousState3 = -1;
int previousState4 = -1;
int previousState5 = -1;
int currentState1 = -1;
int currentState2 = -1;
int currentState3 = -1;
int currentState4 = -1;
int currentState5 = -1;
unsigned long previousMillis = 0;

void checkSensor()
{
  unsigned long currentMillis = millis();
  // Check sensor data every 250 milliseconds
  if (currentMillis - previousMillis >= 5) {
    // Check the sensor state and send data when it changes.
    currentState1 = digitalRead(motionSensorPin1);
    currentState2 = digitalRead(motionSensorPin2);
    currentState3 = digitalRead(motionSensorPin3);
    currentState4 = digitalRead(motionSensorPin4);
    currentState5 = digitalRead(motionSensorPin5);
    if (currentState1 != previousState1) {
      Cayenne.virtualWrite(VIRTUAL_PIN1, currentState1);
      previousState1 = currentState1;
    }
    else if (currentState2 != previousState2) {
      Cayenne.virtualWrite(VIRTUAL_PIN2, currentState2);
      previousState2 = currentState2;
    }
    else if (currentState3 != previousState3) {
      Cayenne.virtualWrite(VIRTUAL_PIN3, currentState3);
      previousState3 = currentState3;
    }
    else if (currentState4 != previousState4) {
      Cayenne.virtualWrite(VIRTUAL_PIN4, currentState4);
      previousState4 = currentState4;
    }
    else if (currentState5 != previousState5) {
      Cayenne.virtualWrite(VIRTUAL_PIN5, currentState5);
      previousState5 = currentState5;
    }
    previousMillis = currentMillis;
  }
}


