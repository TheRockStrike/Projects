#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#include <CayenneSerial.h>

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

// CS pin is used for software or hardware SPI
#define CAP1188_CS  10

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

char token[] = "4v10ay143a";

void setup() {
  Serial.begin(9600);
  Serial.println("CAP1188 test!");
  Cayenne.begin(token);
  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");

}

void loop() {
  uint8_t touched = cap.touched();
  Cayenne.run();
  for (uint8_t i = 0; i < 8; i++) {
    if (touched & (1 << i)) {
      if (i == 0) {
        Cayenne.virtualWrite(V0, 1);
      }
      else if (i == 1) {
        Cayenne.virtualWrite(V0, 1);
      }
    }
  }

}
