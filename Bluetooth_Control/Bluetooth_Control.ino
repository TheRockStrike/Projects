#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

const int led1 = 4;
const int led2 = 5;
const int buzzer = 6;
const int relay = 7;

const int bluetoothTx = 2; // TX-O pin of bluetooth mate, Arduino D2
const int bluetoothRx = 3; // RX-I pin of bluetooth mate, Arduino D3

LiquidCrystal lcd (13, 12, 11, 10, 9, 8);

int blueTooth;

boolean lightBlink = false;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

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

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  setTime(17, 43, 00, 24, 4, 2016);

  digitalWrite(relay, HIGH);

  Serial.begin(9600); // baud rate

  lcd.begin(16, 2);
  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("                ");

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
}

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


void loop()
{
  String inputBluetooth = "";
  time_t t = now();
  int secondVal = second(t);
  int hourVal = hourFormat12();
  int minuteVal = minute();
  int dayVal = day();
  int monthVal = month();

  while (bluetooth.available() > 0) {
    inputBluetooth += (char) bluetooth.read();
    delay(5);
  }

  if (inputBluetooth == "Led 1 on" || inputBluetooth == "Led 1 on ") {
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                    /--------/                    ");
    Serial.println("                    /        /                    ");
    Serial.println("                    /Led 1 On/                    ");
    Serial.println("                    /        /                    ");
    Serial.println("                    /--------/                    ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Led 1 off" || inputBluetooth == "Led 1 off ") {
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                    /---------/                    ");
    Serial.println("                    /         /                    ");
    Serial.println("                    /Led 1 Off/                    ");
    Serial.println("                    /         /                    ");
    Serial.println("                    /---------/                    ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Led 2 on" || inputBluetooth == "Led 2 on ") {
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                    /--------/                    ");
    Serial.println("                    /        /                    ");
    Serial.println("                    /Led 2 On/                    ");
    Serial.println("                    /        /                    ");
    Serial.println("                    /--------/                    ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Led 2 off" || inputBluetooth == "Led 2 off ")
  {
    digitalWrite(led2, LOW); // off
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                    /---------/                    ");
    Serial.println("                    /         /                    ");
    Serial.println("                    /Led 2 Off/                    ");
    Serial.println("                    /         /                    ");
    Serial.println("                    /---------/                    ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "All" || inputBluetooth == "All ")
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(relay, LOW);
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                 /----------------/                 ");
    Serial.println("                 /                /                 ");
    Serial.println("                 /Everything is On/                 ");
    Serial.println("                 /                /                 ");
    Serial.println("                 /----------------/                 ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Song" || inputBluetooth == "Song ")
  {
    noTone(6);
    // play a note on pin 6 for 200 ms:
    tone(6, 440, 700);
    delay(200);

    // turn off tone function for pin 6:
    noTone(6);
    // play a note on pin 7 for 500 ms:
    tone(6, 494, 700);
    delay(200);

    // turn off tone function for pin 7:
    noTone(6);
    // play a note on pin 8 for 500 ms:
    tone(6, 523, 700);
    delay(200);

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
    delay(1000);
    lcd.clear();
  }
  else if (inputBluetooth == "Relay on" || inputBluetooth == "Relay on ") {
    digitalWrite(relay, LOW);
    tone(6, 560, 100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("The Relay = On");
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                    /--------/                    ");
    Serial.println("                    /        /                    ");
    Serial.println("                    /Relay On/                    ");
    Serial.println("                    /        /                    ");
    Serial.println("                    /--------/                    ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Relay off" || inputBluetooth == "Relay off ") {
    digitalWrite(relay, HIGH);
    tone(6, 560, 100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("The Relay = Off");
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                    /---------/                    ");
    Serial.println("                    /         /                    ");
    Serial.println("                    /Relay Off/                    ");
    Serial.println("                    /         /                    ");
    Serial.println("                    /---------/                    ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Off" || inputBluetooth == "Off ") {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(relay, HIGH);
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

    Serial.print(monthVal);
    Serial.print("/");
    Serial.print(dayVal);
    Serial.print("/");
    Serial.print(year());
    Serial.print("   At   ");
    Serial.print(hourVal);
    Serial.print(":");
    Serial.print(minuteVal);
    Serial.print(":");
    Serial.print(secondVal);
    Serial.println("                                                                 ");
    Serial.println("                 /-----------------/                 ");
    Serial.println("                 /                 /                 ");
    Serial.println("                 /Everything is Off/                 ");
    Serial.println("                 /                 /                 ");
    Serial.println("                 /-----------------/                 ");
    Serial.println("                                                                 ");
  }
  else if (inputBluetooth == "Time" || inputBluetooth == "Time ") {
    tone(6, 450, 100);
    timeDisplay();

  }
}
