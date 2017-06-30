#include <LiquidCrystal.h>
#include <Adafruit_BMP085.h>

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;

LiquidCrystal lcd (13, 12, 11, 10, 9, 8); // Declares the connectivity pins on the LCD Display as connected to Pins 8 - 13.


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  if (!bmp.begin()) {
    Serial.print("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}

void loop() {

  for (int i = 0; i < 20; i++) {
    lcd.setCursor(0, 0);
    lcd.print("Temp = ");
    lcd.print(bmp.readTemperature());
    lcd.print(" *C");

    lcd.setCursor(0, 1);
    lcd.print("Temp = ");
    double tempF = (bmp.readTemperature() * (9.0 / 5.0)) + 32.0;
    lcd.print(tempF);
    lcd.print(" *F");
    delay(500);
  }
  lcd.clear();
  for (int i = 0; i < 20; i++) {
    lcd.setCursor(0, 0);
    lcd.print("Alti. = ");
    lcd.print(bmp.readAltitude());
    lcd.print(" m");

    lcd.setCursor(0, 1);
    lcd.print("Pres. = ");
    lcd.print(bmp.readPressure());
    lcd.print(" Pa");
    delay(500);
  }
  lcd.clear();
}
