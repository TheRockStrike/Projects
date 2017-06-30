#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

// CS pin is used for software or hardware SPI
#define CAP1188_CS  10

boolean isEnabled = false;

// These are defined for software SPI, for hardware SPI, check your
// board's SPI pins in the Arduino documentation
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13

// For I2C, connect SDA to your Arduino's SDA pin, SCL to SCL pin
// On UNO/Duemilanove/etc, SDA == Analog 4, SCL == Analog 5
// On Leonardo/Micro, SDA == Digital 2, SCL == Digital 3
// On Mega/ADK/Due, SDA == Digital 20, SCL == Digital 21

// Use I2C, no reset pin!
Adafruit_CAP1188 cap = Adafruit_CAP1188();

int count = 0;

void setup() {
  Serial.begin(250000);
  Serial.println("CAP1188 test!");
  pinMode(7, OUTPUT);
  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 Found! Initializing...");
  while (count < 50 && isEnabled) {
    Serial.print("*");
    count++;
    delay(100);
  }
  Serial.println();
}

void capNum (int x) {
  switch (x) {
    case 2:
      digitalWrite(x, HIGH);
      break;
    case 3:
      digitalWrite(x, HIGH);
      break;
    case 4:
      digitalWrite(x, HIGH);
      break;
    case 5:
      digitalWrite(x, HIGH);
      break;
    case 6:
      digitalWrite(x, HIGH);
      break;
    case 7:
      digitalWrite(x, HIGH);
      break;
    case 8:
      digitalWrite(x, HIGH);
      break;
    case 9:
      digitalWrite(x, HIGH);
      break;
  }
}

void loop() {

  uint8_t touched = cap.touched();

  if (touched == 0) {
    return;
  }

  for (uint8_t i = 0; i < 8; i++) {
    if (touched & (1 << i)) {
      Serial.print("C"); Serial.print(i + 1); Serial.print("\t");
      capNum(i + 2);
    }
  }
  Serial.println();
  delay(50);
  for (int i = 2; i < 10; i++) {
    digitalWrite(i, LOW);
  }
}
