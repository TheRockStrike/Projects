#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUM_LEDS 5
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  clearStrip(); // Initialize all pixels to 'off'
}

void loop() {
  for (int i = 0; i < 800; i++) {
    Fire(55, 120, 50);
  }
  delay(2000);
  for (int i = 0; i < 5; i++) {
    rainbowCycle(20);
  }
  delay(2000);
  knightRider(3, 32, 4, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  knightRider(3, 32, 3, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  knightRider(3, 32, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  knightRider(3, 32, 5, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  knightRider(3, 32, 6, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  knightRider(3, 32, 7, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  knightRider(3, 32, 8, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  knightRider(3, 32, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
  delay(2000);

  // Iterate through a whole rainbow of colors
  for (byte j = 0; j < 252; j += 7) {
    knightRider(1, 16, 2, colorWheel(j)); // Cycles, Speed, Width, RGB Color
  }
  delay(2000);
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}


void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// Cycles - one cycle is scanning through all pixels left then right (or right then left)
// Speed - how fast one cycle is (32 with 16 pixels is default KnightRider speed)
// Width - how wide the trail effect is on the fading out LEDs.  The original display used
//         light bulbs, so they have a persistance when turning off.  This creates a trail.
//         Effective range is 2 - 8, 4 is default for 16 pixels.  Play with this.
// Color - 32-bit packed RGB color value.  All pixels will be this color.
// knightRider(cycles, speed, width, color);
void knightRider(uint16_t cycles, uint16_t speed, uint8_t width, uint32_t color) {
  uint32_t old_val[NUM_LEDS]; // up to 256 lights!
  // Larson time baby!
  for (int i = 0; i < cycles; i++) {
    for (int count = 1; count < NUM_LEDS; count++) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x > 0; x--) {
        old_val[x - 1] = dimColor(old_val[x - 1], width);
        strip.setPixelColor(x - 1, old_val[x - 1]);
      }
      strip.show();
      delay(speed);
    }
    for (int count = NUM_LEDS - 1; count >= 0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x <= NUM_LEDS ; x++) {
        old_val[x - 1] = dimColor(old_val[x - 1], width);
        strip.setPixelColor(x + 1, old_val[x + 1]);
      }
      strip.show();
      delay(speed);
    }
  }
}

void clearStrip() {
  for ( int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0x000000); strip.show();
  }
}

uint32_t dimColor(uint32_t color, uint8_t width) {
  return (((color & 0xFF0000) / width) & 0xFF0000) + (((color & 0x00FF00) / width) & 0x00FF00) + (((color & 0x0000FF) / width) & 0x0000FF);
}

// Using a counter and for() loop, input a value 0 to 251 to get a color value.
// The colors transition like: red - org - ylw - grn - cyn - blue - vio - mag - back to red.
// Entering 255 will give you white, if you need it.
uint32_t colorWheel(byte WheelPos) {
  byte state = WheelPos / 21;
  switch (state) {
    case 0: return strip.Color(255, 0, 255 - ((((WheelPos % 21) + 1) * 6) + 127)); break;
    case 1: return strip.Color(255, ((WheelPos % 21) + 1) * 6, 0); break;
    case 2: return strip.Color(255, (((WheelPos % 21) + 1) * 6) + 127, 0); break;
    case 3: return strip.Color(255 - (((WheelPos % 21) + 1) * 6), 255, 0); break;
    case 4: return strip.Color(255 - (((WheelPos % 21) + 1) * 6) + 127, 255, 0); break;
    case 5: return strip.Color(0, 255, ((WheelPos % 21) + 1) * 6); break;
    case 6: return strip.Color(0, 255, (((WheelPos % 21) + 1) * 6) + 127); break;
    case 7: return strip.Color(0, 255 - (((WheelPos % 21) + 1) * 6), 255); break;
    case 8: return strip.Color(0, 255 - ((((WheelPos % 21) + 1) * 6) + 127), 255); break;
    case 9: return strip.Color(((WheelPos % 21) + 1) * 6, 0, 255); break;
    case 10: return strip.Color((((WheelPos % 21) + 1) * 6) + 127, 0, 255); break;
    case 11: return strip.Color(255, 0, 255 - (((WheelPos % 21) + 1) * 6)); break;
    default: return strip.Color(0, 0, 0); break;
  }
}

