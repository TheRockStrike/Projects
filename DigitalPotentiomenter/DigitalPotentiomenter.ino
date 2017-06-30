#include <SPI.h>

byte addressRedLED = B00010000;
byte addressGreenLED = B00000000;
int CS = 10; // Chip Select pin is connected to pin 10

void setup()
{
  pinMode (CS, OUTPUT);
  SPI.begin();
}

void loop()
{
  for (int i = 0; i <= 128; i++)
  {
    int j = 128 - i;
    digitalPotWriteRed(i);
    digitalPotWriteGreen(j);
    delay(10);
  }
  delay(2000);
  for (int i = 128; i >= 0; i--)
  {
    int j = 128 - i;
    digitalPotWriteRed(i);
    digitalPotWriteGreen(j);
    delay(10);
  }
}

int digitalPotWriteRed(int valueRed)
{
  digitalWrite(CS, LOW);
  SPI.transfer(addressRedLED);
  SPI.transfer(valueRed);
  digitalWrite(CS, HIGH);
}

int digitalPotWriteGreen(int valueGreen)
{
  digitalWrite(CS, LOW);
  SPI.transfer(addressGreenLED);
  SPI.transfer(valueGreen);
  digitalWrite(CS, HIGH);
}

