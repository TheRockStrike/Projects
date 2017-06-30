#include <Event.h>
#include <Timer.h>

#include <math.h>
#include <Adafruit_NeoPixel.h>

Timer t;

const float val = 2.2;

String colorPick = "PinkBlueRed";

const byte analogPinL         = A0;   // - - - - - - - - - - - - - - - - - left channel analog data from shield
const byte analogPinR         = A1;   // - - - - - - - - - - - - - - - - - right channel analog data from shield
const byte strobePin          = 4;   // - - - - - - - - - - - - - - - - - data strobe for shield
const byte resetPin           = 5;   // - - - - - - - - - - - - - - - - - reset strobe for shield
const byte dataPin            = 11;   // - - - - - - - - - - - - - - - - - data pin for Neopixels
const byte numBand            = 9;  // - - - - - - - - - - - - - - - - - number of LEDs for each freq band
const byte numTop             = 0;   // - - - - - - - - - - - - - - - - - number of LEDs to have top color
const int noise[]             = {2, 2, 2, 2, 2, 2, 2}; // - - - - - - - - set this to magnitude of noise from shield
const float gain[]            = {val, val, val, val, val, val, val}; // - gain for each band
const unsigned long loop_dlay = 1;  // - - - - - - - - - - - - - - - - - loop delay to slow down display update rate

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numBand*7, dataPin, NEO_GRB + NEO_KHZ800);

enum audio {
  MONO,
  RIGHT,
  LEFT };

int spectrumReadR;  //R magnitude from shield
int spectrumReadL;  //L magnitude from shield
int audio = MONO;   //set audio mode to mono, combine R&L channels
int mag = 0;        //the magnitude of a freq band
int numON = 0;      //the number of LEDs on in a freq band
float fl_mag = 0.0; //floating point mag after noise removal and scaling

int peakArray[7];

byte
  peak = 0,
  dotCount = 0;

void setup() { 
  Serial.begin(9600);
  pinMode(resetPin, OUTPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  strip.begin(); //begin strip communication
  strip.show(); //set strip blank
  
  digitalWrite(resetPin,HIGH);
    delayMicroseconds(5);
  digitalWrite(strobePin,HIGH);
    delayMicroseconds(50);              // strobe PW > 18 usec min
  digitalWrite(strobePin,LOW);
    delayMicroseconds(50);              //reset PW > 100 usec min
  digitalWrite(resetPin,LOW);
    delayMicroseconds(5);               
  digitalWrite(strobePin,HIGH);
    delayMicroseconds(100);             // allow reset to strobe falling > 72 usec min

  t.every(100,peakLower);
}

void loop() {
  for(byte band = 1; band <= 7; band++) {
    digitalWrite(strobePin, LOW); //set analyzer to low to read
    delayMicroseconds(40);
    
    spectrumReadR = analogRead(analogPinR); //read right audio
    spectrumReadL = analogRead(analogPinL); //read left audio
    
    digitalWrite(strobePin, HIGH); //set analyzer back to high
    
    mag = (spectrumReadR + spectrumReadL) / 2;
    mag = max(0, (mag - noise[band-1])); //creates magnitude of frequency
    fl_mag = gain[band-1] * float(mag); //adjusts magnitude for gain
    numON = map(fl_mag, 0, 1024, 0, numBand+1); //maps magnitude to number of active pixels

    anyBand(band);
    
    //Serial.print(band);
    //Serial.println(peakArray[band-1]);
    if(peakArray[band-1]==0) strip.setPixelColor(peakArray[band-1] + numBand*(band-1), strip.Color(0,0,0));
    else strip.setPixelColor(peakArray[band-1] + numBand*(band-1), strip.Color(255,0,0));
    t.update();
  }
  strip.setBrightness(255);
  strip.show();

  delay(loop_dlay);
}

void readBand(byte band) {
  for(byte band = 1; band <= 7; band++) {
    digitalWrite(strobePin, LOW); //set analyzer to low to read
    delayMicroseconds(40);
    
    spectrumReadR = analogRead(analogPinR); //read right audio
    spectrumReadL = analogRead(analogPinL); //read left audio
    
    digitalWrite(strobePin, HIGH); //set analyzer back to high
    
    mag = (spectrumReadR + spectrumReadL) / 2; //average L and R spectrums to make mono
    mag = max(0, (mag - noise[band-1])); //creates magnitude of frequency
    fl_mag = gain[band-1] * float(mag); //adjusts magnitude for gain
    numON = map(fl_mag, 0, 1024, 0, numBand+1); //maps magnitude to number of active pixels

    anyBand(band);
  }
}

void anyBand(byte band) {
  for(byte i = 0; i < numBand; i++){
    if(i < (numON - numTop - 1)){
      strip.setPixelColor(i + numBand*(band-1), Wheel(map(i,0,numBand-1,20,83))); //main wheel colors
    }
    else if(i >= numON){
      strip.setPixelColor(i + numBand*(band-1), strip.Color(0,0,0)); //unused colors on wheel
    }
    else{
      if(i > peakArray[band-1]) peakArray[band-1] = i; //used for falling peak dot
    }
  }
}

void peakLower() {
  for(byte i = 0; i < 7; i++) {
    if(peakArray[i] > 0) peakArray[i]--; 
    else continue;
  }
}

uint32_t Wheel(byte WheelPos) {
  //if(colorPick == "GreenYellowRed") {
   // return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  //}
  //else if(colorPick == "PinkPurpleBlue") {
    //return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  //}
  //else if(colorPick == "GreenTealBlue") {
    return strip.Color(0, 255 - WheelPos * 3, WheelPos * 3);
  //}
  //return strip.Color(255 - WheelPos * 3, 0, WheelPos *3);
}
