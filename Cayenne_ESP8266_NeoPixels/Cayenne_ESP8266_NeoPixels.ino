#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"
#include "CayenneWiFiClient.h"
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN D3
#define SECTION_SELECT_PIN V0

#define RED_PIN V1
#define GREEN_PIN V2
#define BLUE_PIN V3
#define PINK_PIN V4
#define MAGENTA_PIN V5
#define TEAL_PIN V6
#define CLEAR_PIN V25


/* Parameter 1 = number of pixels in strip
  // Parameter 2 = Arduino pin number (most are valid)
  // Parameter 3 = pixel type flags, add together as needed:
  //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
  //   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
*/
Adafruit_NeoPixel strip = Adafruit_NeoPixel(29, PIN, NEO_GRB + NEO_KHZ800);

char token[] = "mmlognh31j";
char ssid[] = "BlueTardis_EXT";
char password[] = "Dog188*Uz6";

int sectionStart = 0;
int sectionEnd = 0;

int sectionSelect = 0;

void setup() {
  Serial.begin(115200);
  Cayenne.begin(token, ssid, password);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  Cayenne.run();

  //  strip.show();
}
CAYENNE_IN(RED_PIN)
{
  // get value sent from dashboard
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = sectionStart; i < sectionEnd; i++) {
      strip.setPixelColor(i, 255, 0, 0);
      strip.show();
    }
  }
  else {
    strip.show();
  }
}
CAYENNE_IN(GREEN_PIN)
{
  // get value sent from dashboard
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = sectionStart; i < sectionEnd; i++) {
      strip.setPixelColor(i, 0, 255, 0);
      strip.show();
    }
  }
  else {
    strip.show();
  }
}
CAYENNE_IN(BLUE_PIN)
{
  // get value sent from dashboard
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = sectionStart; i < sectionEnd; i++) {
      strip.setPixelColor(i, 0, 0, 255);
      strip.show();
    }
  }
  else {
    strip.show();
  }
}
CAYENNE_IN(PINK_PIN)
{
  // get value sent from dashboard
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = sectionStart; i < sectionEnd; i++) {
      strip.setPixelColor(i, 255, 20, 147);
      strip.show();
    }
  }
  else {
    strip.show();
  }
}
CAYENNE_IN(MAGENTA_PIN)
{
  // get value sent from dashboard
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = sectionStart; i < sectionEnd; i++) {
      strip.setPixelColor(i, 255, 0, 255);
      strip.show();
    }
  }
  else {
    strip.show();
  }
}
CAYENNE_IN(TEAL_PIN)
{
  // get value sent from dashboard
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = sectionStart; i < sectionEnd; i++) {
      strip.setPixelColor(i, 14, 237, 222);
      strip.show();
    }
  }
  else {
    strip.show();
  }

}
CAYENNE_IN(CLEAR_PIN)
{
  int val = getValue.asInt();
  if (val == 1) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
    }
    //delay(1000);
    Cayenne.virtualWrite(CLEAR_PIN, 0);
  }
  else {
    strip.show();
  }
}

CAYENNE_IN(SECTION_SELECT_PIN)
{
  // get value sent from dashboard
  int reading = getValue.asInt(); // 0 to 1023
  sectionSelect = map(reading, 0, 1023, 0, 1); // Section number
  sectionSelect -= 1;
  if ((strip.numPixels() % 20) != 0) {
    if (sectionSelect == 19) {
      sectionStart = sectionSelect * (strip.numPixels() / 20); // Starting at this pixel
      sectionEnd = strip.numPixels(); // And fill out the rest of the strip
    }
    else {
      sectionStart = sectionSelect * (strip.numPixels() / 20); // Starting at this pixel
      sectionEnd = sectionStart + (strip.numPixels() / 20); // And going to this pixel
    }
  }
}


