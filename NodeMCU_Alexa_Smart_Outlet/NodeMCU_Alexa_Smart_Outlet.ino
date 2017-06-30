#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

#define WIFI_SSID "BlueTardis_EXT"
#define WIFI_PASS "Dog188*Uz6"

#define SERIAL_BAUDRATE                 115200

fauxmoESP fauxmo;

#define RELAY1 16
#define RELAY2 05
#define RELAY3 04
#define RELAY4 10


// Wifi
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}


void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.printf("[MAIN] %s state: %s\n", device_name, state ? "ON" : "OFF");

  if ( (strcmp(device_name, "my LEDs") == 0) ) {
    // adjust the relay immediately!
    if (state) {
      digitalWrite(RELAY1, LOW);
    } else {
      digitalWrite(RELAY1, HIGH);
    }
  }

  if ( (strcmp(device_name, "my bed lights") == 0) ) {
    // adjust the relay immediately!
    if (state) {
      digitalWrite(RELAY2, LOW);
    } else {
      digitalWrite(RELAY2, HIGH);
    }
  }

  if ( (strcmp(device_name, "my desk lamp") == 0) ) {
    // adjust the relay immediately!
    if (state) {
      digitalWrite(RELAY3, LOW);
    } else {
      digitalWrite(RELAY3, HIGH);
    }
  }

  if ( (strcmp(device_name, "four") == 0) ) {
    // adjust the relay immediately!
    if (state) {
      digitalWrite(RELAY4, LOW);
    } else {
      digitalWrite(RELAY4, HIGH);
    }
  }
}

void setup() {
  // Set to outputs
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Set relay all off
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();
  Serial.println("FauxMo demo sketch");
  Serial.println("After connection, ask Alexa to 'turn relay on' or 'off'");

  // Wifi
  wifiSetup();

  // Fauxmo
  fauxmo.addDevice("my LEDs");
  fauxmo.addDevice("my bed lights");
  fauxmo.addDevice("my desk lamp");
  fauxmo.addDevice("four");
  fauxmo.onMessage(callback);
}

void loop() {
  fauxmo.handle();
}

