#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

#define WIFI_SSID "BlueTardis_EXT"
#define WIFI_PASS "Dog188*Uz6"

#define SERIAL_BAUDRATE                 115200

fauxmoESP fauxmo;

#define N_PIXELS  60  // Number of pixels in strand
#define MIC_PIN   A0  // Microphone is attached to this analog pin
#define LED_PIN   04  // NeoPixel LED strand is connected to this pin
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

volatile boolean fireplace_state = false; // off by default!

uint32_t Wheel(byte WheelPos); // function prototype

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------


void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}


void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.printf("[MAIN] %s state: %s\n", device_name, state ? "ON" : "OFF");

  if ( (strcmp(device_name, "the fireplace") == 0) ) {
    // this just sets a variable that the main loop() does something about
    if (state) {
      fireplace_state = true;
    } else {
      fireplace_state = false;
    }
  }

}

void setup() {
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();
  Serial.println("FauxMo demo sketch");
  Serial.println("After connection, ask Alexa/Echo to 'turn pixels on' or 'off' or 'turn relay on' or 'off'");

  // Wifi
  wifiSetup();

  fauxmo.addDevice("the fireplace");
  fauxmo.onMessage(callback);
}

uint8_t j = 0; // color swirl incrementer
void loop() {

  fauxmo.handle();
  if (fireplace_state) {
    uint8_t  i;
    uint16_t minLvl, maxLvl;
    int      n, height;

    n   = analogRead(MIC_PIN);                        // Raw reading from mic
    n   = abs(n - 512 - DC_OFFSET); // Center on zero
    n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
    lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

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
  } else {
    for (int16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
      delay(50);
      strip.show();
    }

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
