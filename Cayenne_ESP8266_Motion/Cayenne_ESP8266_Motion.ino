#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"
#include "CayenneWiFiClient.h"
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

char token[] = "mmlognh31j";
char ssid[] = "BlueTardis_EXT";
char password[] = "Dog188*Uz6";

#define VIRTUAL_PIN1 V1

const int motionSensorPin1 = D0;
int previousState1 = -1;
int currentState1 = -1;
unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(motionSensorPin1, INPUT);
  Cayenne.begin(token, ssid, password);
}
void loop()
{
  Cayenne.run();
  unsigned long currentMillis = millis();
  // Check sensor data every 250 milliseconds
  if (currentMillis - previousMillis >= 5) {
    // Check the sensor state and send data when it changes.
    currentState1 = digitalRead(motionSensorPin1);
    if (currentState1 != previousState1) {
    
      Cayenne.virtualWrite(VIRTUAL_PIN1, currentState1);
      previousState1 = currentState1;
    }
    previousMillis = currentMillis;
  }
}

