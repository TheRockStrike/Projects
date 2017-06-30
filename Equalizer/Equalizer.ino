/*
  LED VU meter for Arduino and Adafruit NeoPixel LEDs.

  Hardware requirements:
  - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
  - Adafruit Electret Microphone Amplifier (ID: 1063)
  - Adafruit Flora RGB Smart Pixels (ID: 1260)
   OR
  - Adafruit NeoPixel Digital LED strip (ID: 1138)
  - Optional: battery for portable use (else power through USB or adapter)
  Software requirements:
  - Adafruit NeoPixel library

  Connections:
  - 3.3V to mic amp +
  - GND to mic amp -
  - Analog pin to microphone output (configurable below)
  - Digital pin to LED data input (configurable below)
  See notes in setup() regarding 5V vs. 3.3V boards - there may be an
  extra connection to make and one line of code to enable or disable.
*/

#include <Adafruit_NeoPixel.h>
#include <CayenneSerial.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "23zw5ba8aw";

#define N_PIXELS  65  // Number of pixels in strand
#define MIC_PIN   A0  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET 0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     200  // Noise/hum/interference in mic signal
#define SAMPLES   1  // Length of buffer for dynamic level adjustment
#define TOP       (N_PIXELS + 2) // Allow dot to go slightly off scale
#define PEAK_FALL 30  // Rate of peak falling dot

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

int inputPin = A3;
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

void loop() {
  Cayenne.run();

  potReading = (analogRead(inputPin) / 30) * 2;
  buttonReading = digitalRead(smoothingEnableButton);

  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;

  n   = analogRead(MIC_PIN);                        // Raw reading from mic
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
  height += potReading;

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

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
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

