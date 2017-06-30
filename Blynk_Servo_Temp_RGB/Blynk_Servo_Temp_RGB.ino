/**************************************************************
   Blynk is a platform with iOS and Android apps to control
   Arduino, Raspberry Pi and the likes over the Internet.
   You can easily build graphic interfaces for all your
   projects by simply dragging and dropping widgets.

     Downloads, docs, tutorials: http://www.blynk.cc
     Blynk community:            http://community.blynk.cc
     Social networks:            http://www.fb.com/blynkapp
                                 http://twitter.com/blynk_app

   Blynk library is licensed under MIT license
   This example code is in public domain.

 **************************************************************
   Rotate a servo using a slider!

   App project setup:
     Slider widget (0...180) on V3

 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX
#include <SimpleTimer.h>
#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleStream.h>
#include <Servo.h>

#include <Adafruit_NeoPixel.h>
#define PIN 10
#define NUM_LEDS 5
#define BRIGHTNESS 255// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "21ff32adc0d3476a80c2c6393a018e64";


float temp;
const int tempPin = A0; //analog pin 1
const int motionSensorPin1 = 53;
const int motionSensorPin2 = 52;
const int motionSensorPin3 = 51;

int previousState1 = -1;
int previousState2 = -1;
int previousState3 = -1;
int previousState4 = -1;
int previousState5 = -1;
int currentState1 = -1;
int currentState2 = -1;
int currentState3 = -1;
int currentState4 = -1;
int currentState5 = -1;
unsigned long previousMillis = 0;

SimpleTimer timer;

WidgetLED led1(V1); //register to virtual pin 1
WidgetLED led2(V2); //register to virtual pin 2
WidgetLED led3(V4); //register to virtual pin 3

Servo servo;

void setup()
{
  DebugSerial.begin(9600);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'

  // Blynk will work through Serial
  Serial.begin(9600);
  Blynk.begin(auth, Serial);
  servo.attach(9);
  timer.setInterval(1500L, sendUptime);
  timer.setInterval(500L, checkSensor);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);
}

BLYNK_WRITE(V1) // There is a Widget that WRITEs data to V1
{
  int r = param[0].asInt(); // get a RED channel value
  int g = param[1].asInt(); // get a GREEN channel value
  int b = param[2].asInt(); // get a BLUE channel value
  setLEDColor(r, g, b);
}

void setLEDColor(int r, int g, int b) {
  analogWrite(5, r);
  analogWrite(6, g);
  analogWrite(7, b);
}

void sendUptime()
{
  // shows the value temp on virtual pin 10
  Blynk.virtualWrite(V0, temp);
}

BLYNK_WRITE(V3)
{
  servo.write(param.asInt());
}

void checkSensor()
{
  unsigned long currentMillis = millis();
  // Check sensor data every 250 milliseconds
  if (currentMillis - previousMillis >= 5) {
    // Check the sensor state and send data when it changes.
    currentState1 = digitalRead(motionSensorPin1);
    currentState2 = digitalRead(motionSensorPin2);
    currentState3 = digitalRead(motionSensorPin3);
    if (currentState1 != previousState1) {
      led1.on();
      previousState1 = currentState1;
    }
    else if (currentState2 != previousState2) {
      led2.on();
      previousState2 = currentState2;
    }
    else if (currentState3 != previousState3) {
      led3.on();
      previousState3 = currentState3;
    }
    else {
      led1.off();
      led2.off();
      led3.off();
    }
    previousMillis = currentMillis;
  }
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
  temp = analogRead(tempPin) * 5.0;
  temp /= 1024.0;
  temp = (temp - 0.5) * 100;
  temp = (temp * (9.0 / 5.0)) + 32.0;

  if (digitalRead(13) == HIGH) {
    strip.setBrightness(255);
    neopixels();
  }
  else if (digitalRead(13) == LOW) {
    strip.setBrightness(0);
    strip.show();
  }
}

void neopixels() {
  strip.setPixelColor(0, 0, 255, 0);
  strip.setPixelColor(1, 255, 128, 0);
  strip.setPixelColor(2, 255, 0, 255);
  strip.setPixelColor(3, 0, 0, 204);
  strip.setPixelColor(4, 0, 255, 255);
  strip.show();
}

