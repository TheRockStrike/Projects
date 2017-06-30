/*
  House digital LED program
  2014 Carl Sutter
  Arduino Mega connected to a string of WS2812B digital LEDs on pin 6
  Uses the Adafruit NeoPixel library - please support them!

  There is a bunch of old an possibly untested code in here - left as an example
*/

#include <Adafruit_NeoPixel.h>
#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(74+74 + 52+27+39+71 + 9+38+39+11 + 15+70+15+11+40, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //thanksgiving();
  halloween1();
  xmas();
  xmas2();
  heartbeat();
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  rainbow(20);
  rainbowCycle(20);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// rainbow
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

//chase
void chase(int width, int gap, int loop_delay, int fR, int fG, int fB, int bR, int bG, int bB) {
  int len = width + gap;
  int R, G, B;
  int i, j;

  for (i = 0; i < strip.numPixels(); i++) {
    for (j = 0; j <= i; j++) {
      R = fR - (((i - j) % len) * fR / width);
      if (R  < 0) R = bR;
      G = fG - (((i - j) % len) * fG / width);
      if (G  < 0) G = bG;
      B = fB - (((i - j) % len) * fB / width);
      if (B  < 0) B = bB;
      strip.setPixelColor(j, strip.Color(R, G, B));
    }
    strip.show();
    delay(loop_delay);
  }
} // chase


void xmas_colors() {
  int mod;

  for (uint16_t i = 0; i < strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(0, 0, 0));

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    mod = i % 30;
    if ((mod >= 0) && (mod <= 2)) strip.setPixelColor(i, strip.Color(255, 0, 0));
    if ((mod >= 6) && (mod <= 8)) strip.setPixelColor(i, strip.Color(0, 255, 0));
    if ((mod >= 12) && (mod <= 14)) strip.setPixelColor(i, strip.Color(204, 50, 0));
    if ((mod >= 18) && (mod <= 20)) strip.setPixelColor(i, strip.Color(0, 0, 255));
    if ((mod >= 24) && (mod <= 26)) strip.setPixelColor(i, strip.Color(255, 240, 0));
  }
  strip.show();
} // xmas_colors


// dim the color up
void dim_up(int R, int G, int B, int wait) {
  int j;

  for (j = 0; j < 256; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(R * j / 256, G * j / 256, B * j / 256));
    }
    strip.show();
    delay(wait);
  }
} // dim_up


void chase_single(int wait, int width, int fR, int fG, int fB, int bR, int bG, int bB) {
  // green dot moving down
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    if ((i - width) >= 0) {
      strip.setPixelColor(i - width, strip.Color(bR, bG, bB));
    }
    strip.setPixelColor(i, strip.Color(fR, fG, fB));
    strip.show();
    delay(wait);
  }
} // chase_single


void chase_single_back(int wait, int width, int fR, int fG, int fB, int bR, int bG, int bB) {
  // green dot moving down
  for (uint16_t i = strip.numPixels() - 1; i >= 0; i--) {
    if ((i + width) <= strip.numPixels()) {
      strip.setPixelColor(i + width, strip.Color(bR, bG, bB));
    }
    strip.setPixelColor(i, strip.Color(fR, fG, fB));
    strip.show();
    delay(wait);
  }
}


// Fill the dots one after the other with a color
void xmas() {
  int j, k;

  //chase_single(25, 5, 255, 0, 0, 0, 255, 0);
  //chase_single_back(25, 5, 255, 0, 0, 0, 255, 0);

  xmas_colors();
  delay(10000);

  chase(3, 10, 75, 255, 0, 0, 0, 100, 0);
  chase(3, 10, 75, 0, 255, 0, 100, 0, 0);

  dim_up(255, 0, 0, 10);
  dim_up(0, 255, 0, 10);
  dim_up(255, 0, 0, 10);
  dim_up(0, 255, 0, 10);
  //dim_up(255, 0, 0, 10);

  chase_single(25, 5, 255, 0, 0, 0, 255, 0);
  //chase_single_back(25, 5, 255, 0, 0, 0, 255, 0);

  dim_up(0, 0, 255, 10);
  delay(5000);
  chase(3, 10, 75, 255, 255, 255, 0, 0, 255);

} // xmas

void red_white() {
#define pat_width 7
  int i, j, k;

  strip.setBrightness(0);

  for (j = 0; j < 256; j++) {
    strip.setBrightness(j);
    for (i = 0; i < strip.numPixels(); i++) {
      if (((i % pat_width) == 0) || ((i % pat_width) == 1)) strip.setPixelColor(i, 255, 0, 0);
      if (((i % pat_width) == 2) || ((i % pat_width) == 3)) strip.setPixelColor(i, 255, 255, 255);
      if (((i % pat_width) == 4) || ((i % pat_width) == 5) || ((i % pat_width) == 6)) strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    delay(10);
  }

  for (k = 0; k < 200; k++) {
    for (i = 0; i < strip.numPixels(); i++) {
      j = i + k % pat_width;
      if (((j % pat_width) == 0) || ((j % pat_width) == 1)) strip.setPixelColor(i, 255, 0, 0);
      if (((j % pat_width) == 2) || ((j % pat_width) == 3)) strip.setPixelColor(i, 255, 255, 255);
      if (((j % pat_width) == 4) || ((j % pat_width) == 5) || ((j % pat_width) == 6)) strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    delay(100);
  }
} // red_white

void sparkle(uint32_t col1, uint32_t col2) {
  uint16_t i, j, k;

  for (i = 0; i < strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(0, 0, 0));

  for (k = 0; k < 400; k++) {
    i = random(strip.numPixels());
    j = random(2);
    if (j == 0) strip.setPixelColor(i, col1);
    if (j == 1) strip.setPixelColor(i, col2);
    strip.show();
    delay(20);
    strip.setPixelColor(i, 0);
  }

} // sparkle


void xmas2() {
  sparkle(0xff0000, 0x00ff00);
  red_white();
  //sparkle(0x0000ff, 0xffffff);
  chase(3, 10, 75, 255, 0, 0, 0, 100, 0);
  chase(3, 10, 75, 0, 255, 0, 100, 0, 0);
  xmas_colors();
  delay(8000);
  dim_up(0, 0, 255, 10);
  delay(5000);
  chase(3, 10, 75, 255, 255, 255, 0, 0, 255);
} // xmas2


// Fill the dots one after the other with a color
#define r_orange 204
#define g_orange 50
#define b_orange 0
#define r_purple 150
#define g_purple 0
#define b_purple 150
void halloween1() {
  int n2 = strip.numPixels() / 2;
  int j;
#define num_steps 50

  //for(uint16_t i=0; i<strip.numPixels(); i++) {
  //    strip.setPixelColor(i, strip.Color(r_orange, g_orange, b_orange));
  // }
  //strip.show();
  //delay(500);

  // orange and red
  for (j = 0; j < num_steps; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r_orange * j / num_steps, g_orange * j / num_steps, b_orange * j / num_steps));
    }
    delay(10);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(n2 - i, strip.Color(255, 0, 0));
    strip.setPixelColor(n2 + i + 1, strip.Color(255, 0, 0));
    delay(20);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(i, strip.Color(r_orange, g_orange, b_orange));
    strip.setPixelColor(strip.numPixels() - i, strip.Color(r_orange, g_orange, b_orange));
    delay(20);
    strip.show();
  }
  delay(500);

  for (j = num_steps; j > 0; j--) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r_orange * j / num_steps, g_orange * j / num_steps, b_orange * j / num_steps));
    }
    delay(10);
    strip.show();
  }
  delay(500);


 // purple and green
  for (j = 0; j < num_steps; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r_purple * j / num_steps, g_purple * j / num_steps, b_purple * j / num_steps));
    }
    delay(10);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(n2 - i, strip.Color(0, 100, 0));
    strip.setPixelColor(n2 + i + 1, strip.Color(0, 100, 0));
    delay(20);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(n2 - i, strip.Color(r_purple, g_purple, b_purple));
    strip.setPixelColor(n2 + i + 1, strip.Color(r_purple, g_purple, b_purple));
    delay(20);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(n2 - i, strip.Color(r_orange, g_orange, b_orange));
    strip.setPixelColor(n2 + i + 1, strip.Color(r_orange, g_orange, b_orange));
    delay(20);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(i, strip.Color(r_purple, g_purple, b_purple));
    strip.setPixelColor(strip.numPixels() - i, strip.Color(r_purple, g_purple, b_purple));
    delay(20);
    strip.show();
  }
  delay(500);

  for (uint16_t i = 0; i <= n2; i++) {
    strip.setPixelColor(i, strip.Color(r_purple, g_purple, b_purple));
    strip.setPixelColor(strip.numPixels() - i, strip.Color(r_purple, g_purple, b_purple));
    delay(20);
    strip.show();
  }
  delay(500);

  for (j = num_steps; j > 0; j--) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r_purple * j / num_steps, g_purple * j / num_steps, b_purple * j / num_steps));
    }
    delay(10);
    strip.show();
  }
  delay(500);

  heartbeat();
  delay(500);


 // fade black up orange
  for (j = 0; j < num_steps; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r_orange * j / num_steps, g_orange * j / num_steps, b_orange * j / num_steps));
    }
    delay(10);
    strip.show();
  }
  delay(500);
  // move a black gap
#define gap_size 5
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    for (j = 0; j < gap_size; j++) {
      strip.setPixelColor(i + j, strip.Color(0, 0, 0));
      strip.setPixelColor(i - j, strip.Color(r_orange, g_orange, b_orange));
    }
    strip.show();
    delay(10);
  }
  strip.show();
  delay(500);

  for (uint16_t i = strip.numPixels() - 1; i > 0; i--) {
    for (j = 0; j < gap_size; j++) {
      strip.setPixelColor(i + j, strip.Color(r_orange, g_orange, b_orange));
      strip.setPixelColor(i - j, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(10);
  }
  strip.show();
  delay(500);

  // fade orange to black
  for (j = num_steps; j > 0; j--) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r_orange * j / num_steps, g_orange * j / num_steps, b_orange * j / num_steps));
    }
    delay(10);
    strip.show();
  }
  delay(500);

} // halloween1


void heartbeat() {
  // heartbeat
  beat();
  delay(500);
  beat();
  delay(500);
  beat();
  delay(500);
  beat();
  delay(500);
  beat();
  delay(500);
  beat();
  delay(500);
  beat();
  delay(500);
  beat();
  delay(500);
}


void beat() {
#define heart_steps 20
  int j;

  for (j = 0; j < heart_steps; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255 * j / heart_steps, 0, 0));
    }
    //delay(5);
    strip.show();
  }
  delay(150);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(170);

  for (j = 0; j < heart_steps; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255 * j / heart_steps, 0, 0));
    }
    //delay(5);
    strip.show();
  }
  delay(150);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(200);

}  // beat

// THANKSGIVING
#define r_orange 204
#define g_orange 50
#define b_orange 0

#define r_yellow 255
#define g_yellow 240
#define b_yellow 0

#define r_brown 151
#define g_brown 69
#define b_brown 0

void thanksgiving() {
  int n2 = strip.numPixels() / 2;
  int j;
#define num_steps 50

  // orange
  set_color(r_orange, g_orange, b_orange);
  //delay(1000);

  //fade to yellow
  for (j = 0; j < num_steps; j++) {
    set_color(r_orange + j * (r_yellow - r_orange) / num_steps, g_orange + j * (g_yellow - g_orange) / num_steps, b_orange + j * (b_yellow - b_orange) / num_steps);
    strip.show();
    delay(50);
  }
  //delay(1000);

  // yellow
  set_color(r_yellow, g_yellow, b_yellow);
  //delay(1000);

  //fade to brown
  for (j = 0; j < num_steps; j++) {
    set_color(r_yellow + j * (r_brown - r_yellow) / num_steps, g_yellow + j * (g_brown - g_yellow) / num_steps, b_yellow + j * (b_brown - b_yellow) / num_steps);
    strip.show();
    delay(50);
  }
  //delay(1000);

  // brown
  set_color(r_brown, g_brown, b_brown);
  //delay(1000);

  //fade to orange
  for (j = 0; j < num_steps; j++) {
    set_color(r_brown + j * (r_orange - r_brown) / num_steps, g_brown + j * (g_orange - g_brown) / num_steps, b_brown + j * (b_orange - b_brown) / num_steps);
    strip.show();
    delay(50);
  }
  //delay(1000);

} // thanksgiving

// set_color
void set_color(int r, int g, int b) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
} // set_color
