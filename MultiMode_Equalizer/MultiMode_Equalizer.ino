#include <Adafruit_NeoPixel.h>
#include <CayenneSerial.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "23zw5ba8aw";

#define N_PIXELS  60  // Number of pixels in strand
#define MIC_PIN   A1  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define LED_HALF  N_PIXELS/2 // For EqualizerV2
#define DC_OFFSET 0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     200  // Noise/hum/interference in mic signal
#define SAMPLES   75  // Length of buffer for dynamic level adjustment
#define TOP       (N_PIXELS + 2) // Allow dot to go slightly off scale
#define PEAK_FALL 30  // Rate of peak falling dot
#define KNOB_PIN  A2  //Pin for the trimpot 10K

// Fill the dots one after the other with a color
#define r_orange 204
#define g_orange 50
#define b_orange 0
#define r_purple 150
#define g_purple 0
#define b_purple 150

byte
peak      = 0,      // Used for falling dot
dotCount  = 0,      // Frame counter for delaying dot-falling speed
volCount  = 0;      // Frame counter for storing past volume data
int
vol[SAMPLES],       // Collection of prior volume samples
    lvl       = 10,      // Current "dampened" audio level
    minLvlAvg = 0,      // For dynamic adjustment of graph low & high
    maxLvlAvg = 512;

Adafruit_NeoPixel
strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

int potReading = 0;

const int numReadings = 30;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total

int smoothingEnableButton = 5;
int state = HIGH;      // the current state of the output pin
int buttonReading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long pinTime = 0;         // the last time the output pin was toggled         \ These are longs because the time, measured in miliseconds,
long debounce = 200;   // the debounce time, increase if the output flickers  / will quickly become a bigger number than can be stored in an int.

uint16_t gradient = 0; //Used to iterate and loop through each color palette gradually

uint8_t volume = 0;    //Holds the volume level read from the sound detector.
uint8_t last = 0;      //Holds the value of volume from the previous loop() pass.

float maxVol = 15;     //Holds the largest volume recorded thus far to proportionally adjust the visual's responsiveness.
float knob = 1023.0;   //Holds the percentage of how twisted the trimpot is. Used for adjusting the max brightness.
float avgVol = 0;      //Holds the "average" volume-level to proportionally adjust the visual experience.
float avgBump = 0;     //Holds the "average" volume-change to trigger a "bump."

bool bump = true;     //Used to pass if there was a "bump" in volume

void setup() {

  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  // analogReference(EXTERNAL);

  memset(vol, 0, sizeof(vol));
  strip.begin();

  pinMode(smoothingEnableButton, INPUT);

  //Serial.begin(9600); // Cayenne will not connect if serial is enabled.

  Cayenne.begin(token);//Baud rate can be specified by calling Cayenne.begin(token, 9600);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}
void equalizerV2() {
  volume = analogRead(MIC_PIN);       //Record the volume level from the sound detector
  knob = analogRead(KNOB_PIN) / 1023.0; //Record how far the trimpot is twisted
  avgVol = (avgVol + volume) / 2.0;     //Take our "average" of volumes.

  //Sets a threshold for volume.
  //  In practice I've found noise can get up to 15, so if it's lower, the visual thinks it's silent.
  //  Also if the volume is less than average volume / 2 (essentially an average with 0), it's considered silent.
  if (volume < avgVol / 2.0 || volume < 15) volume = 0;

  //If the current volume is larger than the loudest value recorded, overwrite
  if (volume > maxVol) maxVol = volume;

  //This is where "gradient" is reset to prevent overflow.
  if (gradient > 1529) {

    gradient %= 1530;

    //Everytime a palette gets completed is a good time to readjust "maxVol," just in case
    //  the song gets quieter; we also don't want to lose brightness intensity permanently
    //  because of one stray loud sound.
    maxVol = (maxVol + volume) / 2.0;
  }

  //If there is a decent change in volume since the last pass, average it into "avgBump"
  if (volume - last > avgVol - last && avgVol - last > 0) avgBump = (avgBump + (volume - last)) / 2.0;

  //if there is a notable change in volume, trigger a "bump"
  bump = (volume - last) > avgBump;

  Pulse();   //Calls the visual to be displayed with the globals as they are.

  gradient++;    //Increments gradient

  last = volume; //Records current volume for next pass

  delay(30);   //Paces visuals so they aren't too fast to be enjoyable
}
void equalizer() {
  Cayenne.run();

  potReading = analogRead(KNOB_PIN);
  potReading = map(potReading, 0, 1023, 0, N_PIXELS);
  buttonReading = digitalRead(smoothingEnableButton);

  //Serial.println(potReading);

  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;

  n   = analogRead(MIC_PIN);                        // Raw reading from mic
  //Serial.println(analogRead(MIC_PIN));
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
  //Serial.println(n);
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)
  //Serial.println(lvl);

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
  if (N_PIXELS >= 100) {
    height *= N_PIXELS / 100;
  }
  height += potReading;
  //Serial.println(height);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (buttonReading == HIGH && previous == LOW && millis() - pinTime > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    pinTime = millis();
  }

  previous = buttonReading;
  //Serial.println(state);

  if (state == true) {
    // subtract the last reading:

    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = height;
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    height = total / numReadings;
    //Serial.println(height); // send it to the computer as ASCII digits
  }
  if (height < 0L)       height = 0;     // Clip output
  else if (height > TOP) height = TOP;
  if (height > peak)     peak   = height; // Keep 'peak' dot at top


  // Color pixels based on rainbow gradient
  for (i = 0; i < N_PIXELS; i++) {
    if (i >= height)               strip.setPixelColor(i,   0,   0, 0);
    else strip.setPixelColor(i, Wheel(map(i, 0, strip.numPixels() - 1, 30, 1000)));

  }



  // Draw peak dot
  if (peak > 0 && peak <= N_PIXELS - 1) strip.setPixelColor(peak, Wheel(map(peak, 0, strip.numPixels() - 1, 30, 150)));

  strip.show(); // Update strip

  // Every few frames, make the peak pixel drop by 1:

  if (++dotCount >= PEAK_FALL) { //fall rate

    if (peak > 0) peak--;
    dotCount = 0;
  }



  vol[volCount] = n;                      // Save sample for dynamic leveling
  if (++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for (i = 1; i < SAMPLES; i++) {
    if (vol[i] < minLvl)      minLvl = vol[i];
    else if (vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)

  /*
    Serial.println("\nHeight: ");
    Serial.print(height);
    Serial.println("\nPotentiometer Reading: ");
    Serial.print(potReading);
    Serial.println("\nRaw Mic Reading: ");
    Serial.print(n);
  */
}
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[N_PIXELS];
  int cooldown;
  Cayenne.run();
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < N_PIXELS; i++) {
    cooldown = random(0, ((Cooling * 10) / N_PIXELS) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = N_PIXELS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < N_PIXELS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}
void loop() {
  Cayenne.run();
}
CAYENNE_IN(V0) {
  int value = getValue.asInt(); // Get value as integer
  while (value == 1) {
    xmas();
    Cayenne.run();
  }
}
CAYENNE_IN(V1) {
  int value = getValue.asInt(); // Get value as integer
  while (value == 1) {
    equalizer();
    Cayenne.run();
  }
}
CAYENNE_IN(V2) {
  int value = getValue.asInt(); // Get value as integer
  while (value == 1) {
    Fire(55, 120, 50);
    Cayenne.run();
  }
}
CAYENNE_IN(V3) {
  int value = getValue.asInt(); // Get value as integer
  while (value == 1) {
    halloween1();
    Cayenne.run();
  }
}
CAYENNE_IN(V4) {
  int value = getValue.asInt(); // Get value as integer
  while (value == 1) {
    Cayenne.run();
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
      delay(50);
    }
  }
}
CAYENNE_IN(V5) {
  int value = getValue.asInt(); // Get value as integer
  while (value == 1) {
    equalizerV2();
    Cayenne.run();
  }
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
  for (int i = 0; i < N_PIXELS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
uint32_t Wheel(byte WheelPos) {
  // Input a value 0 to 255 to get a color value.
  // The colors are a transition r - g - b - back to r.
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
}
void dim_up(int R, int G, int B, int wait) {
  int j;

  for (j = 0; j < 256; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(R * j / 256, G * j / 256, B * j / 256));
    }
    strip.show();
    delay(wait);
  }
}
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
}
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
void xmas() {
  int j, k;
  Cayenne.run();
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

}
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
}
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

}
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
}
void halloween1() {
  Cayenne.run();
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

}
void heartbeat() {
  // heatbeat
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

}
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
}
void colorWipe(uint32_t c, uint8_t wait) {
  // Fill the dots one after the other with a color
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void Pulse() {

  fade(0.75);   //Listed below, this function simply dims the colors a little bit each pass of loop()

  //Advances the gradient to the next noticeable color if there is a "bump"
  if (bump) gradient += 64;

  //If it's silent, we want the fade effect to take over, hence this if-statement
  if (volume > 0) {
    uint32_t col = Rainbow(gradient); //Our retrieved 32-bit color

    //These variables determine where to start and end the pulse since it starts from the middle of the strip.
    //  The quantities are stored in variables so they only have to be computed once.
    int start = LED_HALF - (LED_HALF * (volume / maxVol));
    int finish = LED_HALF + (LED_HALF * (volume / maxVol)) + strip.numPixels() % 2;
    //Listed above, LED_HALF is simply half the number of LEDs on your strip. ↑ this part adjusts for an odd quantity.

    for (int i = start; i < finish; i++) {

      //"damp" creates the fade effect of being dimmer the farther the pixel is from the center of the strip.
      //  It returns a value between 0 and 1 that peaks at 1 at the center of the strip and 0 at the ends.
      float damp = float(
                     ((finish - start) / 2.0) -
                     abs((i - start) - ((finish - start) / 2.0))
                   )
                   / float((finish - start) / 2.0);

      //Sets the each pixel on the strip to the appropriate color and intensity
      //  strip.Color() takes 3 values between 0 & 255, and returns a 32-bit integer.
      //  Notice "knob" affecting the brightness, as in the rest of the visuals.
      //  Also notice split() being used to get the red, green, and blue values.
      strip.setPixelColor(i, strip.Color(
                            split(col, 0) * pow(damp, 2.0) * knob,
                            split(col, 1) * pow(damp, 2.0) * knob,
                            split(col, 2) * pow(damp, 2.0) * knob
                          ));
    }
    //Sets the max brightness of all LEDs. If it's loud, it's brighter.
    //  "knob" was not used here because it occasionally caused minor errors in color display.
    strip.setBrightness(255.0 * pow(volume / maxVol, 2));
  }

  //This command actually shows the lights. If you make a new visualization, don't forget this!
  strip.show();
}
void fade(float damper) {
  //Fades lights by multiplying them by a value between 0 and 1 each pass of loop().
  //"damper" must be between 0 and 1, or else you'll end up brightening the lights or doing nothing.
  if (damper >= 1) damper = 0.99;

  for (int i = 0; i < strip.numPixels(); i++) {

    //Retrieve the color at the current position.
    uint32_t col = (strip.getPixelColor(i)) ? strip.getPixelColor(i) : strip.Color(0, 0, 0);

    //If it's black, you can't fade that any further.
    if (col == 0) continue;

    float colors[3]; //Array of the three RGB values

    //Multiply each value by "damper"
    for (int j = 0; j < 3; j++) colors[j] = split(col, j) * damper;

    //Set the dampened colors back to their spot.
    strip.setPixelColor(i, strip.Color(colors[0] , colors[1], colors[2]));
  }
}
uint8_t split(uint32_t color, uint8_t i ) {

  //0 = Red, 1 = Green, 2 = Blue

  if (i == 0) return color >> 16;
  if (i == 1) return color >> 8;
  if (i == 2) return color >> 0;
  return -1;
}
uint32_t Rainbow(unsigned int i) {
  //This function simply take a value and returns a gradient color
  //  in the form of an unsigned 32-bit integer

  //The gradient returns a different, changing color for each multiple of 255
  //  This is because the max value of any of the 3 LEDs is 255, so it's
  //  an intuitive cutoff for the next color to start appearing.
  //  Gradients should also loop back to their starting color so there's no jumps in color.

  if (i > 1529) return Rainbow(i % 1530);
  if (i > 1274) return strip.Color(255, 0, 255 - (i % 255));   //violet -> red
  if (i > 1019) return strip.Color((i % 255), 0, 255);         //blue -> violet
  if (i > 764) return strip.Color(0, 255 - (i % 255), 255);    //aqua -> blue
  if (i > 509) return strip.Color(0, 255, (i % 255));          //green -> aqua
  if (i > 255) return strip.Color(255 - (i % 255), 255, 0);    //yellow -> green
  return strip.Color(255, i, 0);                               //red -> yellow
}
