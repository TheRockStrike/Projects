#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"
#include "CayenneWiFiClient.h"
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

char token[] = "mmlognh31j";
char ssid[] = "BlueTardis_EXT";
char password[] = "Dog188*Uz6";

// Defines Tirg and Echo pins of the Ultrasonic Sensor
const int trigPin = D0;
const int echoPin = D1;
// Variables for the duration and the distance
long duration;
int distance;

const int numReadings = 50;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(250000);
  Cayenne.begin(token, ssid, password);
}
void loop() {
  Cayenne.run();

  distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
  
  if (distance < 30) {
    Cayenne.virtualWrite(V15, 1);
  }
  else if (distance > 70) {
    int temp = distance; // wonky readings, thrown out
  }
  else {
    Cayenne.virtualWrite(V15, 0);
  }
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = distance;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  Cayenne.virtualWrite(V16, average);
}  

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;
  return distance;
}
