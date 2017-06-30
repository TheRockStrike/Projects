/*
   This project emulates WeMo light switches for use with the Amazon Alexa.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks
// When adding another device, add both the ON and the OFF callback functions here.
void lightOneOn();
void lightOneOff();
void lightTwoOn();
void lightTwoOff();
void outletOneOn();
void outletOneOff();
void outletTwoOn();
void outletTwoOff();

// Change this before you flash
const char* ssid = "BlueTardis_EXT";
const char* password = "Dog188*Uz6";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

// When adding another device, add another one of these.
Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *outletOne = NULL;
Switch *outletTwo = NULL;

// Set Relay Pins
int relayOne = 14;
int relayTwo = 15;
int relayThree = 03;
int relayFour = 01;

void setup()
{
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  //Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Show WiFi status along with SSID of network
    Serial.println("WiFi Connected: ");
    Serial.print(ssid);

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port number, on callback, off callback
    // When adding another device, duplicate one of these with a new name, port, and both callback methods.
    lightOne = new Switch("Fan One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    outletOne = new Switch("Outlet One", 82, outletOneOn, outletOneOff);
    outletTwo = new Switch("Outlet Two", 83, outletTwoOn, outletTwoOff);

    // When adding another device, also call the addDevice function with its name.
    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    upnpBroadcastResponder.addDevice(*outletOne);
    upnpBroadcastResponder.addDevice(*outletTwo);

    //Set relay pins to outputs
    pinMode(relayOne, OUTPUT);
    pinMode(relayTwo, OUTPUT);
    pinMode(relayThree, OUTPUT);
    pinMode(relayFour, OUTPUT);

    //Set each relay pin to LOW
    digitalWrite(relayOne, LOW);   // sets relayOne off
    digitalWrite(relayTwo, LOW);   // sets relayOne off
    digitalWrite(relayThree, LOW);   // sets relayOne off
    digitalWrite(relayFour, LOW);   // sets relayOne off
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    // When adding another device, also duplicate this server loop here.
    lightOne->serverLoop();
    lightTwo->serverLoop();
    outletOne->serverLoop();
    outletTwo->serverLoop();
  }
}

// When adding another device, add both of its callback functions here.

void lightOneOn() {
  digitalWrite(relayOne, HIGH);   // sets relayOne on
  Serial.println("Relay One: On");
}
void lightOneOff() {
  // Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, LOW);   // sets relayOne off
  Serial.println("Relay One: Off");
}
void lightTwoOn() {
  // Serial.print("Switch 2 turn on ...");
  digitalWrite(relayThree, HIGH);   // sets relayTwo on
  Serial.println("Relay Three: On");
}
void lightTwoOff() {
  // Serial.print("Switch 2 turn off ...");
  digitalWrite(relayThree, LOW);   // sets relayTwo Off
  Serial.println("Relay Three: Off");
}
void outletOneOn() {
  //  Serial.print("Socket 1 turn on ...");
  digitalWrite(relayFour, HIGH);   // sets relayThree on
  Serial.println("Relay Four: On");
}
void outletOneOff() {
  // Serial.print("Socket 1 turn off ...");
  digitalWrite(relayFour, LOW);   // sets relayThree off
  Serial.println("Relay Four: Off");
}

void outletTwoOn() {
  // Serial.print("Socket 2 turn on ...");
  digitalWrite(relayTwo, HIGH);   // sets relayFour on
  Serial.println("Relay Two: On");
}

void outletTwoOff() {
  // Serial.print("Socket 2 turn off ...");
  digitalWrite(relayTwo, LOW);   // sets relayFour off
  Serial.println("Relay Two: Off");
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
