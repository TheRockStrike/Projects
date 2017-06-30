#include <SoftwareSerial.h> // Library for BlueTooth communication.

const int bluetoothTx = 3; // Transmit (TX-O) pin of bluetooth module, connected to Pin 3 on the Arduino.
const int bluetoothRx = 4; // Receive (RX-I) pin of bluetooth module, connected to Pin 4 on the Arduino.
const int speaker = 5; // The Piezo buzzer is connected to Pin 6 on the Arduino.

const int relay_1 = 9;
const int relay_2 = 10;
const int relay_3 = 11;
const int relay_4 = 12;

boolean lightBlink = false; // Once connected, the LED on the BT module will not blink.

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx); // Passes two parameters to the SoftwareSerial method, which initializes the BlueTooth protocol.

void setup() {
  bluetooth.begin(115200); // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$"); // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$"); // Enter command mode
  delay(100); // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N"); // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600); // Start bluetooth serial at 9600

  pinMode(speaker, OUTPUT);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);

  digitalWrite(relay_1, HIGH); // Turn relay 1 off
  digitalWrite(relay_2, HIGH); // Turn relay 2 off
  digitalWrite(relay_3, HIGH); // Turn relay 3 off
  digitalWrite(relay_4, HIGH); // Turn relay 4 off
}

void loop() {
  String inputBluetooth = "";

  while (bluetooth.available() > 0) {
    inputBluetooth += (char) bluetooth.read();
    delay(5);
  }
  if (inputBluetooth == "Relay 1 on" || inputBluetooth == "Relay 1 on ") {
    digitalWrite(relay_1, LOW); // Turn relay 1 on
    tone(5, 560, 100);
  }
  else if (inputBluetooth == "Relay 1 off" || inputBluetooth == "Relay 1 off ") {
    digitalWrite(relay_1, HIGH); // Turn relay 1 off
    tone(5, 300, 100);
  }
  else if (inputBluetooth == "Relay 2 on" || inputBluetooth == "Relay 2 on ") {
    digitalWrite(relay_2, LOW); // Turn relay 2 on
    tone(5, 560, 100);
  }
  else if (inputBluetooth == "Relay 2 off" || inputBluetooth == "Relay 2 off ") {
    digitalWrite(relay_2, HIGH); // Turn relay 2 off
    tone(5, 300, 100);
  }
  else if (inputBluetooth == "Relay 3 on" || inputBluetooth == "Relay 3 on ") {
    digitalWrite(relay_3, LOW); // Turn relay 3 on
    tone(5, 560, 100);
  }
  else if (inputBluetooth == "Relay 3 off" || inputBluetooth == "Relay 3 off ") {
    digitalWrite(relay_3, HIGH); // Turn relay 3 off
    tone(5, 300, 100);
  }
  else if (inputBluetooth == "Relay 4 on" || inputBluetooth == "Relay 4 on ") {
    digitalWrite(relay_4, LOW); // Turn relay 4 on
    tone(5, 560, 100);
  }
  else if (inputBluetooth == "Relay 4 off" || inputBluetooth == "Relay 4 off ") {
    digitalWrite(relay_4, HIGH); // Turn relay 4 off
    tone(5, 300, 100);
  }
  else if (inputBluetooth == "All" || inputBluetooth == "All ") {
    digitalWrite(relay_1, LOW); // Turn relay 1 on
    digitalWrite(relay_2, LOW); // Turn relay 2 on
    digitalWrite(relay_3, LOW); // Turn relay 3 on
    digitalWrite(relay_4, LOW); // Turn relay 4 on
    tone(5, 560, 100);
  }
  else if (inputBluetooth == "Off" || inputBluetooth == "Off ") {
    digitalWrite(relay_1, HIGH); // Turn relay 1 off
    digitalWrite(relay_2, HIGH); // Turn relay 2 off
    digitalWrite(relay_3, HIGH); // Turn relay 3 off
    digitalWrite(relay_4, HIGH); // Turn relay 4 off
    tone(5, 200, 100);
  }
  else {}
}
