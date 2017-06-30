#include <Chrono.h>
#include <LightChrono.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

Chrono seconds = (Chrono::SECONDS);
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel rings you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int minutes = 1;
int hours = 1;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  ring.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
}

void loop() {

  if (seconds.elapsed() > 59) {
    minutes++;
    seconds.restart();
  }
  if (minutes > 59) {
    minutes = 1;
    hours++;
  }
  if (hours > 12) {
    hours = 1;
  }
  Serial.print("Seconds: ");
  Serial.println(seconds.elapsed());
  Serial.print("Minutes: ");
  Serial.println(minutes);
  //delay(1000);

  clockFace(seconds.elapsed());

}

void clockFace(int hr) {
  switch (hr) {
    case 1:
      ring.clear();
      ring.setPixelColor(hr, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 1, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 2:
      ring.clear();
      ring.setPixelColor(hr + 1, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 2, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 3:
      ring.clear();
      ring.setPixelColor(hr + 2, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 3, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 4:
      ring.clear();
      ring.setPixelColor(hr + 3, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 4, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 5:
      ring.clear();
      ring.setPixelColor(hr + 4, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 5, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 6:
      ring.clear();
      ring.setPixelColor(hr + 5, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 6, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 7:
      ring.clear();
      ring.setPixelColor(hr + 6, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 7, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 8:
      ring.clear();
      ring.setPixelColor(hr + 7, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 8, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 9:
      ring.clear();
      ring.setPixelColor(hr + 8, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 9, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 10:
      ring.clear();
      ring.setPixelColor(hr + 9, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 10, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 11:
      ring.clear();
      ring.setPixelColor(hr + 10, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(hr + 11, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    case 12:
      ring.clear();
      ring.setPixelColor(hr + 11, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.setPixelColor(0, Wheel(map(hr, 0, ring.numPixels() - 1, 30, 1000)));
      ring.show();
      break;
    default:
      ring.clear();
      ring.show();
  }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
