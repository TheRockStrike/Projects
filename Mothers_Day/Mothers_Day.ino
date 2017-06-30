#include <LiquidCrystal.h> // Library for the LCD Display.

LiquidCrystal lcd (13, 12, 11, 10, 9, 8); // Declares the connectivity pins on the LCD Display as connected to Pins 8 - 13.

const int buzzer = 6; // The Piezo buzzer is connected to Pin 6 on the Arduino.

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

const float C = 261.62;
const float D = 293.66;
const float E = 329.63;
const float F = 349.23;
const float G = 391.99;
const float A = 440.00;
const float B = 493.88;

void setup() {
  pinMode(buzzer, OUTPUT);

  lcd.begin(16, 2);
  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);

  // Prints Initializing... on the top row of the screen as the progress bar executes
  lcd.setCursor(1, 0);
  lcd.print("Happy Mother's");
  lcd.setCursor(6, 1);
  lcd.print("Day!");
  delay(3000);

  // For loop for the progress bar
  for (int i = 0; i < 16; i++)
  {

    for (int j = 0; j < 5; j = j + 2)
    {
      lcd.setCursor(i, 1);
      lcd.write(j);
      lcd.setCursor(i, 0);
      lcd.write(j);
      delay(100);
    }
  }
  delay(3000);
  lcd.setCursor(5, 1);
  lcd.print("Wait...");
  tone(buzzer, 300, 100);

  for (int i = 15; i >= 12; i--) {
    lcd.setCursor(i, 1);
    lcd.print(" ");
    delay(100);
  }

  for (int i = 4; i >= 0; i--) {
    lcd.setCursor(i, 1);
    lcd.print(" ");
    delay(100);
  }

  for (int i = 15; i >= 0; i--) {
    lcd.setCursor(i, 0);
    lcd.print(" ");
    delay(100);
  }
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T");
  delay(200);
  tone(buzzer, 100, 100);
  lcd.setCursor(1, 0);
  lcd.print("h");
  delay(200);
  tone(buzzer, 150, 100);
  lcd.setCursor(2, 0);
  lcd.print("a");
  delay(200);
  tone(buzzer, 200, 100);
  lcd.setCursor(3, 0);
  lcd.print("t");
  delay(200);
  tone(buzzer, 250, 100);
  lcd.setCursor(4, 0);
  lcd.print("'");
  delay(200);
  tone(buzzer, 300, 100);
  lcd.setCursor(5, 0);
  lcd.print("s");
  delay(200);
  tone(buzzer, 350, 100);
  lcd.setCursor(7, 0);
  lcd.print("K");
  delay(200);
  tone(buzzer, 400, 100);
  lcd.setCursor(8, 0);
  lcd.print("i");
  delay(200);
  tone(buzzer, 450, 100);
  lcd.setCursor(9, 0);
  lcd.print("n");
  delay(200);
  tone(buzzer, 500, 100);
  lcd.setCursor(10, 0);
  lcd.print("d");
  delay(200);
  tone(buzzer, 550, 100);
  lcd.setCursor(11, 0);
  lcd.print("a");
  delay(200);
  tone(buzzer, 600, 100);
  lcd.setCursor(12, 0);
  lcd.print("...");
  delay(500);
  tone(buzzer, 650, 100);
  lcd.setCursor(0, 1);
  lcd.print("B");
  delay(200);
  tone(buzzer, 700, 100);
  lcd.setCursor(1, 1);
  lcd.print("o");
  delay(200);
  tone(buzzer, 750, 100);
  lcd.setCursor(2, 1);
  lcd.print("r");
  delay(200);
  tone(buzzer, 800, 100);
  lcd.setCursor(3, 1);
  lcd.print("i");
  delay(200);
  tone(buzzer, 850, 100);
  lcd.setCursor(4, 1);
  lcd.print("n");
  delay(200);
  tone(buzzer, 900, 100);
  lcd.setCursor(5, 1);
  lcd.print("g");
  delay(2500);
  lcd.clear();


  tone(buzzer, A, 100);
  lcd.setCursor(0, 0);
  lcd.print("Let's make it a");
  lcd.setCursor(0, 1);
  lcd.print("little more...");
  delay(2500);
  lcd.clear();
  delay(500);

  tone(buzzer, B, 100);
  lcd.setCursor(0, 0);
  lcd.print("Interesting...");
  delay(2500);
  lcd.clear();
  delay(500);


  tone(buzzer, E, 4000);
  lcd.setCursor(0, 0);
  lcd.print("long loveVal = ");
  lcd.setCursor(0, 1);
  lcd.print("digitalRead(Mom)");
  delay(4000);
  lcd.clear();
  delay(500);

  tone(buzzer, C, 4000);
  lcd.setCursor(0, 0);
  lcd.print("Mom = FrankTemp");
  lcd.setCursor(0, 1);
  lcd.print("Manage.risk();");
  delay(4000);
  lcd.clear();
  delay(500);

  tone(buzzer, D, 4000);
  lcd.setCursor(0, 0);
  lcd.print("Compiler Error!");
  delay(3500);
  lcd.clear();
  delay(500);

  tone(buzzer, G, 4000);
  lcd.setCursor(0, 0);
  lcd.print("FrankTemp = ");
  lcd.setCursor(0, 1);
  lcd.print("fullOfIdiots(?);");
  delay(4000);
  lcd.clear();
  delay(500);

  tone(buzzer, 130.81, 4000);
  lcd.setCursor(0, 0);
  lcd.print("#define allOfUs");
  lcd.setCursor(0, 1);
  lcd.print("you > any1Else");
  delay(4000);
  lcd.clear();
  delay(500);


  tone(buzzer, F, 4000);
  lcd.setCursor(0, 0);
  lcd.print("weLoveYou = true");
  lcd.setCursor(0, 1);
  lcd.print("love > 111010101");
  delay(4000);
  lcd.clear();
  delay(500);

  tone(buzzer, E, 4000);
  lcd.setCursor(0, 0);
  lcd.print("if (bestEver) {");
  lcd.setCursor(0, 1);
  lcd.print("return weLoveYou");
  delay(4000);
  lcd.clear();
  delay(500);

  tone(buzzer, 247.50, 4000);
  lcd.setCursor(1, 0);
  lcd.print("Happy Mother's");
  lcd.setCursor(3, 1);
  lcd.print("Day, Mom!!");
  delay(4000);
  lcd.clear();
  delay(500);

  tone(buzzer, C, 5000);
  lcd.setCursor(2, 0);
  lcd.print("WE LOVE YOU!");
  for (int i = 0; i < 16; i++)
  {

    for (int j = 0; j < 5; j = j + 2)
    {
      lcd.setCursor(i, 1);
      lcd.write(j);
      delay(100);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
