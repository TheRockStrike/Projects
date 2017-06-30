/*
  Kenny Akers
  Mr. Paige
  Intro to CS
  4/8/16

  In this project, an extra digit (7-segment display) is added to the counter circuit.
  Using multiplexing, 9 pins in total are utilized to create the illusion that both
  displays are on simultaneously, when in fact they alternate being ON and OFF.

  PIN ASSIGNMENTS
   Bottom Pin = Pin 5
   Bottom Left Pin = Pin 6
   Bottom Right Pin = Pin 7
   Middle Pin = Pin 8
   Upper Left Pin = Pin 9
   Upper Right Pin = Pin 10
   Top Pin = Pin 11

   Multiplexing (GND Control) for the first digit = Pin 3
   Multiplexing (GND Control) for the second digit = Pin 4

   Green Button = Pin 12
   Red Button = Pin 13
*/
//Constant declarations of which segments are on which pins
const int bottomPin = 5;
const int bottomLeftPin = 6;
const int bottomRightPin = 7;
const int middlePin = 8;
const int upperLeftPin = 9;
const int upperRightPin = 10;
const int topPin = 11;

//Multiplexing declarations
const int groundPinDigitOne = 3;
const int groundPinDigitTwo = 4;

//Button declarations
const int greenButtonPin = 12;
const int redButtonPin = 13;

//Defines and initializes each variable to have a value of 0
//These are global variables because else if they were local, each time the program
//called void loop() the state and previous state would reset to 0.

int greenButtonState = 0; //Variable to store the current state of the green button
int redButtonState = 0; //Variable to store the current state of the red button
int previousGreenButtonState = 0; //Variable to store the previous state of the green button.

//Declares "count" as a global variable to be used later in the loop method
//to incriment the display. Its initial value is -1 because by default (without pressing
//the button) the display will show a 0 because it will call Case 0 in the method after
//incrimenting by one. Each time the button is pressed after, it will call delse ifferent cases.
int count = -1;

void setup() {
  //Initializes each segment to be an output
  pinMode(bottomPin, OUTPUT);
  pinMode(bottomLeftPin, OUTPUT);
  pinMode(bottomRightPin, OUTPUT);
  pinMode(middlePin, OUTPUT);
  pinMode(upperLeftPin, OUTPUT);
  pinMode(upperRightPin, OUTPUT);
  pinMode(topPin, OUTPUT);

  //Initializes both buttons as INPUT_PULLUPs
  pinMode(greenButtonPin, INPUT_PULLUP);
  pinMode(redButtonPin, INPUT_PULLUP);

  //Start a serial connection for debugging later on
  Serial.begin(9600);
}

void loop() {

  greenButtonState = digitalRead(greenButtonPin); //Stores the value of the button in "currentGreenButtonState"

  redButtonState = digitalRead(redButtonPin); //Stores the value of the button in "currentRedButtonState"

  if (greenButtonState != previousGreenButtonState) { //else if a change in button state occurs, do the following:

    if (greenButtonState == LOW) { //Check to see else if that state is LOW (pressed).
      count++;
      if (count == 0) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 1) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 2) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, LOW);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 3) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 4) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 5) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, LOW);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 6) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, LOW);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 7) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 8) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 9) {
        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 10) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 11) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 12) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, LOW);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 13) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 14) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 15) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, LOW);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 16) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, LOW);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 17) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 18) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 19) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, HIGH);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

      else if (count == 20) {
        digitalWrite(bottomPin, LOW);
        digitalWrite(bottomLeftPin, LOW);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, LOW);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, LOW);
        digitalWrite(groundPinDigitOne, LOW);
        digitalWrite(groundPinDigitTwo, HIGH);
        delay(50);

        digitalWrite(bottomPin, HIGH);
        digitalWrite(bottomLeftPin, HIGH);
        digitalWrite(bottomRightPin, HIGH);
        digitalWrite(middlePin, LOW);
        digitalWrite(upperLeftPin, HIGH);
        digitalWrite(upperRightPin, HIGH);
        digitalWrite(topPin, HIGH);
        digitalWrite(groundPinDigitOne, HIGH);
        digitalWrite(groundPinDigitTwo, LOW);
        delay(50);
      }

    }
    else {
      //Otherwise, don't do anything
    }
    delay(20);
  }
  //Stores the current button value in "previousButtonState" for the next time the program loops through.
  previousGreenButtonState = greenButtonState;

}

