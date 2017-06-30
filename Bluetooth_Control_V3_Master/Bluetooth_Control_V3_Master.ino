#include <Wire.h>
#include <Time.h> // Supplemental library for the TimeLib library.
#include <TimeLib.h> // Library for keeping time.
#include <SoftwareSerial.h> // Library for BlueTooth communication.
#include <LiquidCrystal.h> // Library for the LCD Display.

const int led1 = 4; // The first LED is connected to Pin 4 on the Arduino.
const int led2 = 5; // The second LED is connected to Pin 5 on the Arduino.
const int buzzer = 6; // The Piezo buzzer is connected to Pin 6 on the Arduino.

const int bluetoothTx = 2; // Transmit (TX-O) pin of bluetooth module, connected to Pin 2 on the Arduino.
const int bluetoothRx = 3; // Receive (RX-I) pin of bluetooth module, connected to Pin 3 on the Arduino.

int incorrectPasswordCount = 0;

LiquidCrystal lcd (13, 12, 11, 10, 9, 8); // Declares the connectivity pins on the LCD Display as connected to Pins 8 - 13.

boolean lightBlink = false; // Once connected, the LED on the BT module will not blink.

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx); // Passes two parameters to the SoftwareSerial method, which initializes the BlueTooth protocol.

// Stores data for each pixel, painting each 8x5 square for the progress bar
byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

void setup()
{
  bluetooth.begin(115200); // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$"); // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$"); // Enter command mode
  delay(100); // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N"); // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600); // Start bluetooth serial at 9600

  for (int pin = 4; pin <= 6; pin++) { // Loop to initialize Pins 4 - 7 as outputs (For the LEDs, Relay, and Buzzer).
    pinMode(pin, OUTPUT);
  }

  Wire.begin();

  //Hour, Minute, Second, Day,  Month,  Year
  setTime(16, 53, 00, 1, 7, 2016); // Set the inital time

  // Paints each column of pixels to create the progress bar
  lcd.begin(16, 2);

  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);

  // Prints Initializing... on the top row of the screen as the progress bar executes
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  // For loop for the progress bar
  for (int i = 0; i < 16; i++)
  {

    for (int j = 0; j < 5; j = j + 2)
    {
      lcd.setCursor(i, 1);
      lcd.write(j);
      delay(100);
    }
  }

  lcd.clear();
  for (int i = 0; i < 4; i++) {
    for (int x = 0; x < 5; x++) {
      lcd.setCursor(i, 0);
      lcd.write(x);
      delay(50);
    }
  }
  lcd.setCursor(4, 0);
  lcd.print("B");
  delay(200);
  lcd.setCursor(5, 0);
  lcd.print("l");
  delay(200);
  lcd.setCursor(6, 0);
  lcd.print("u");
  delay(200);
  lcd.setCursor(7, 0);
  lcd.print("D");
  delay(200);
  lcd.setCursor(8, 0);
  lcd.print("u");
  delay(200);
  lcd.setCursor(9, 0);
  lcd.print("i");
  delay(200);
  lcd.setCursor(10, 0);
  lcd.print("n");
  delay(200);
  lcd.setCursor(11, 0);
  lcd.print("o");
  delay(200);

  for (int i = 12; i < 16; i++) {
    for (int x = 0; x < 5; x++) {
      lcd.setCursor(i, 0);
      lcd.write(x);
      delay(50);
    }
  }
  lcd.setCursor(2, 1);
  lcd.print("Kenny  Akers");

  for (int i = 0; i < 2; i++) {
    for (int x = 0; x < 5; x++) {
      lcd.setCursor(i, 1);
      lcd.write(x);
      delay(50);
    }
  }
  for (int i = 7; i < 9; i++) {
    for (int x = 0; x < 5; x++) {
      lcd.setCursor(i, 1);
      lcd.write(x);
      delay(50);
    }
  }
  for (int i = 14; i < 16; i++) {
    for (int x = 0; x < 5; x++) {
      lcd.setCursor(i, 1);
      lcd.write(x);
      delay(50);
    }
  }

  tone(buzzer, 440.00);
  delay(200);
  tone(buzzer, 493.88);
  delay(200);
  tone(buzzer, 523.24);
  delay(200);
  noTone(buzzer);

  lcd.clear();
}

// Custom method for displaying the current time
void timeDisplay() {
  time_t t = now();
  int secondVal = second(t);
  int hourVal = hourFormat12();
  int minuteVal = minute();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  The Time Is:  ");
  lcd.setCursor(5, 1);
  lcd.print(hourVal);
  lcd.print(":");
  lcd.print(minuteVal);

  if (isAM() == true) {
    lcd.setCursor(10, 1);
    lcd.print("AM");
  }

  else if (isPM() == true) {
    lcd.setCursor(10, 1);
    lcd.print("PM");
  }

  else {}
}

void playTetris() {
  // Note frequency declarations
  const float C = 261.62;
  const float D = 293.66;
  const float E = 329.63;
  const float F = 349.23;
  const float G = 391.99;
  const float A = 440.00;
  const float B = 493.88;

  // Tetris - Korobeiniki by Nikolay Nekrasov
  tone(buzzer, E * 2);
  delay(400);
  tone(buzzer, B);
  delay(200);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, D * 2);
  delay(400);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, B);
  delay(200);
  tone(buzzer, A);
  delay(400);
  noTone(buzzer);
  delay(25);
  tone(buzzer, A);
  delay(200);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, E * 2);
  delay(400);
  tone(buzzer, D * 2);
  delay(200);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, B);
  delay(600);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, D * 2);
  delay(400);
  tone(buzzer, E * 2);
  delay(400);
  tone(buzzer, C * 2);
  delay(400);
  noTone(buzzer);
  delay(15);
  tone(buzzer, A);
  delay(400);
  noTone(buzzer);
  delay(25);
  tone(buzzer, A);
  delay(400);
  noTone(buzzer);
  delay(600);
  tone(buzzer, D * 2);
  delay(400);
  tone(buzzer, F * 2);
  delay(200);
  tone(buzzer, A * 2);
  delay(400);
  tone(buzzer, G * 2);
  delay(200);
  tone(buzzer, F * 2);
  delay(200);
  tone(buzzer, E * 2);
  delay(600);
  noTone(buzzer);
  delay(25);
  tone(buzzer, C * 2);
  delay(200);
  noTone(buzzer);
  delay(25);
  tone(buzzer, E * 2);
  delay(400);
  tone(buzzer, D * 2);
  delay(200);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, B);
  delay(400);
  tone(buzzer, B);
  delay(200);
  tone(buzzer, C * 2);
  delay(200);
  tone(buzzer, D * 2);
  delay(400);
  tone(buzzer, E * 2);
  delay(400);
  tone(buzzer, C * 2);
  delay(400);
  noTone(buzzer);
  delay(15);
  tone(buzzer, A);
  delay(400);
  noTone(buzzer);
  delay(25);
  tone(buzzer, A);
  delay(400);

  noTone(buzzer);
}

void LCDOnly(int whichMode) {

  time_t t = now();
  int secondVal = second(t);
  int hourVal = hourFormat12();
  int minuteVal = minute();
  int dayVal = day();
  int monthVal = month();

  switch (whichMode) {
    case 1: // Led 1 on
      digitalWrite(led1, HIGH); // on
      tone(6, 440, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Led 1 = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 2: // Led 1 off
      digitalWrite(led1, LOW); // off
      tone(6, 440, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Led 1 = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 3: // Led 2 on
      digitalWrite(led2, HIGH);
      tone(6, 500, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Led 2 = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 4: // Led 2 off
      digitalWrite(led2, LOW);
      tone(6, 500, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Led 2 = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 5: // Relay 1 on

      Wire.beginTransmission(9);
      Wire.write('1'); // Turn relay 1 on
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 1 = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 6: // Relay 1 off

      Wire.beginTransmission(9);
      Wire.write('2'); // Turn relay 1 off
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 1 = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 7: // Turn Everything on

      Wire.beginTransmission(9);
      Wire.write('9'); // Turn all relays on
      Wire.endTransmission();
    
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      tone(6, 523, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Everything = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 8: // Turn Everything off

      Wire.beginTransmission(9);
      Wire.write('0'); // Turn all relays off
      Wire.endTransmission();

      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      tone(6, 300, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Everything = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 9: // Song
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Playing a Tune");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      delay(2000);
      lcd.clear();
      playTetris();
      break;
    case 10: // Time
      tone(6, 450, 100);
      timeDisplay();
      break;
    case 11:

      Wire.beginTransmission(9);
      Wire.write('3'); // Turn relay 2 on
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 2 = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 12:

      Wire.beginTransmission(9);
      Wire.write('4'); // Turn relay 2 off
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 2 = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 13:

      Wire.beginTransmission(9);
      Wire.write('5'); // Turn relay 3 on
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 3 = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 14:

      Wire.beginTransmission(9);
      Wire.write('6'); // Turn relay 3 off
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 3 = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;

    case 15:

      Wire.beginTransmission(9);
      Wire.write('7'); // Turn relay 4 on
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 4 = On");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    case 16:

      Wire.beginTransmission(9);
      Wire.write('8'); // Turn relay 4 off
      Wire.endTransmission();

      tone(6, 560, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay 4 = Off");
      lcd.setCursor(0, 1);
      lcd.print(monthVal);
      lcd.print("/");
      lcd.print(dayVal);
      lcd.print("/");
      lcd.print("16");
      lcd.print("  ");
      lcd.print(hourVal);
      lcd.print(":");
      lcd.print(minuteVal);
      lcd.print(":");
      lcd.print(secondVal);
      break;
    default:
      // Do nothing
      break;
  }
}


void mainMethod() {

  String inputBluetooth = "";

  while (bluetooth.available() > 0) {
    inputBluetooth += (char) bluetooth.read();
    delay(5);
  }
  if (inputBluetooth == "Led 1 on" || inputBluetooth == "Led 1 on ") {
    LCDOnly(1);
  }
  else if (inputBluetooth == "Led 1 off" || inputBluetooth == "Led 1 off ") {
    LCDOnly(2);
  }
  else if (inputBluetooth == "Led 2 on" || inputBluetooth == "Led 2 on ") {
    LCDOnly(3);
  }
  else if (inputBluetooth == "Led 2 off" || inputBluetooth == "Led 2 off ") {
    LCDOnly(4);
  }

  //////////////////////////////////////////////////////////////////////////////////
  else if (inputBluetooth == "Relay 1 on" || inputBluetooth == "Relay 1 on ") {
    LCDOnly(5);
  }
  else if (inputBluetooth == "Relay 1 off" || inputBluetooth == "Relay 1 off ") {
    LCDOnly(6);
  }
  else if (inputBluetooth == "Relay 2 on" || inputBluetooth == "Relay 2 on ") {
    LCDOnly(11);
  }
  else if (inputBluetooth == "Relay 2 off" || inputBluetooth == "Relay 2 off ") {
    LCDOnly(12);
  }
  else if (inputBluetooth == "Relay 3 on" || inputBluetooth == "Relay 3 on ") {
    LCDOnly(13);
  }
  else if (inputBluetooth == "Relay 3 off" || inputBluetooth == "Relay 3 off ") {
    LCDOnly(14);
  }
  else if (inputBluetooth == "Relay 4 on" || inputBluetooth == "Relay 4 on ") {
    LCDOnly(15);
  }
  else if (inputBluetooth == "Relay 4 off" || inputBluetooth == "Relay 4 off ") {
    LCDOnly(16);
  }
  //////////////////////////////////////////////////////////////////////////////////

  else if (inputBluetooth == "All" || inputBluetooth == "All ") {
    LCDOnly(7);
  }
  else if (inputBluetooth == "Off" || inputBluetooth == "Off ") {
    LCDOnly(8);
  }
  else if (inputBluetooth == "Song" || inputBluetooth == "Song ") {
    LCDOnly(9);
  }
  else if (inputBluetooth == "Time" || inputBluetooth == "Time ") {
    LCDOnly(10);
  }
  else {
  }
}

void loop()
{
  String inputBluetooth = "";
  String answer;

  while (bluetooth.available() > 0) {
    inputBluetooth += (char) bluetooth.read();
    delay(5);
  }
  if (inputBluetooth == "test") {
    tone(6, 440, 100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Accessed Granted");
    delay(1500);
    lcd.clear();
    answer = inputBluetooth;
    while (answer == "test") {
      mainMethod();
    }
  }
  else {
    lcd.setCursor(1, 0);
    lcd.print("Enter Password");
  }
}

