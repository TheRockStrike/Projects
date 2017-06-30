 /*
Cayenne Ethernet Manual IP Example

This sketch connects to the Cayenne server using an Arduino Ethernet Shield W5100 and a 
manually specified MAC address, IP and gateway.

The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the token variable to match the Arduino token from the Dashboard.
2. Compile and upload this sketch.

For Cayenne Dashboard widgets using digital or analog pins this sketch will automatically
send data on those pins to the Cayenne server. If the widgets use Virtual Pins, data
should be sent to those pins using virtualWrites. Examples for sending and receiving
Virtual Pin data are under the Basics folder.
*/

//#define CAYENNE_DEBUG             // Uncomment to show debug messages
//#define CAYENNE_PRINT Serial        // Comment this out to disable prints and save space
#include <CayenneEthernet.h>        // Comment this out if you uncomment a different Ethernet device below.
//#include <CayenneEthernetW5500.h> // Uncomment this and comment out CayenneEthernet.h to use an Ethernet 2 shield or other Ethernet W5500 shield.
                                    // You will need the Ethernet2 library installed. See the ArduinoEthernetW5500 example sketch for more info.
//#include <CayenneEthernetW5200.h> // Uncomment this and comment out CayenneEthernet.h to use an Ethernet W5200 shield.
                                    // You will need the EthernetW5200 library installed. See the ArduinoEthernetW5200 example sketch for more info.

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "3et72usrqo";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress arduino_ip(192, 168, 1, 206);
IPAddress dns_ip(8, 8, 8, 8);
IPAddress gateway_ip(192, 168, 1, 254);
IPAddress subnet_mask(255, 255, 255, 0);

void setup()
{
  //Serial.begin(9600);
  Cayenne.begin(token, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
}

void loop()
{
  Cayenne.run();
}

