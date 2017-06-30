#include <Wire.h>

const int relay_1 = 2;
const int relay_2 = 3;
const int relay_3 = 4;
const int relay_4 = 5;

int x = 0;

void setup() {
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);

  digitalWrite(relay_1, HIGH); // Turn relay 1 off
  digitalWrite(relay_2, HIGH); // Turn relay 2 off
  digitalWrite(relay_3, HIGH); // Turn relay 3 off
  digitalWrite(relay_4, HIGH); // Turn relay 4 off

  Wire.begin(9);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  x = Wire.read();
}

void loop() {
  if (x == '1') {
    digitalWrite(relay_1, LOW); // Turn relay 1 on
  }
  else if (x == '2') {
    digitalWrite(relay_1, HIGH); // Turn relay 1 off
  }
  else if (x == '3') {
    digitalWrite(relay_2, LOW); // Turn relay 2 on
  }
  else if (x == '4') {
    digitalWrite(relay_2, HIGH); // Turn relay 2 off
  }
  else if (x == '5') {
    digitalWrite(relay_3, LOW); // Turn relay 3 on
  }
  else if (x == '6') {
    digitalWrite(relay_3, HIGH); // Turn relay 3 off
  }
  else if (x == '7') {
    digitalWrite(relay_4, LOW); // Turn relay 4 on
  }
  else if (x == '8') {
    digitalWrite(relay_4, HIGH); // Turn relay 4 off
  }
  else if (x == '9') {
    digitalWrite(relay_1, LOW); // Turn relay 1 on
    digitalWrite(relay_2, LOW); // Turn relay 2 on
    digitalWrite(relay_3, LOW); // Turn relay 3 on
    digitalWrite(relay_4, LOW); // Turn relay 4 on
  }
  else if (x == '0') {
    digitalWrite(relay_1, HIGH); // Turn relay 1 off
    digitalWrite(relay_2, HIGH); // Turn relay 2 off
    digitalWrite(relay_3, HIGH); // Turn relay 3 off
    digitalWrite(relay_4, HIGH); // Turn relay 4 off
  }
  else {}

}
