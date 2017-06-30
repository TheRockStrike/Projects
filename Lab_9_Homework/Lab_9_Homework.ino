/*
  Kenny Akers
  Mr. Paige
  Intro to CS
  4/20/16

  In this lab, we are writing to a speaker to play tones with two parameters:
  frequency and duration.

  PIN ASSIGNEMENTS

  Speaker = Pin 10
  Button = Pin 12
*/

const int speaker = 10; //Speaker on pin 10
const int button = 12; //Button on pin 12

int currentButtonState = 0; //Variable to store the current state of the button
int previousButtonState = 0; //Variable to store the previous state of the button

void setup() {
  pinMode(speaker, OUTPUT); //Declares the speaker as an output
  pinMode(button, INPUT_PULLUP); //Declares the button as an INPUT_PULLUP

  Serial.begin(9600);
}
long periodInMicroseconds(float frequency) { //Function method that calculates the period in microseconds and takes one parameter.
  int result;
  for (int i = 1; i <= frequency; i++) {
    result = 1 / frequency;
  }
  return result;
}

void play(float frequency, float duration) { //Method to write to the speaker with a specified frequency and duration, both of which
                                             //are passed to the for loop through the two parameters the method takes.
  for (int i = 1; i <= duration; i++) {
    digitalWrite(speaker, HIGH); //For half of the period, turn the speaker on
    delay(periodInMicroseconds(frequency) / 2);
    digitalWrite(speaker, LOW);  //For the other half of the period, turn the speaker off
    delay(periodInMicroseconds(frequency) / 2);
  }
  //Print out the details of the command in the serial monitor
  Serial.println("Frequency: ");
  Serial.print(frequency);
  Serial.println("Duration: ");
  Serial.print(duration);
}

void loop() {
  /* This portion of code is for Exercise 2, which will play the note only when the button is pressed.
     For now, however, it will remain commented out.

    currentButtonState = digitalRead(button); //Stores the value of the button in "currentButtonState"

    if (currentButtonState != previousButtonState) {

    //We know that the state has changed, now...what is that state?
    if (currentButtonState == LOW) { //Check to see if that state is LOW (pressed).
      play(440.0, 0.500);
      delay(500);
    }
    else {
      //Otherwise, don't do anything
    }
    delay(20); //Delay for button debounce
    }
  */
  /*
    This portion of code is for Exercise 3, which will play the frequency entered into the serial monitor.
    This will remain commented out as well.
    if (Serial.available() > 0) { //If something has been entered into the serial monitor, do the following:
      float incomingData = Serial.parseFloat(); //Store the entered number in the "incomingData" variable
      play(incomingData, 0.500); //Pass the value that's in "incomingData" to the play method with a half second duration.
    }
    else {
      //Otherwise, don't do anything
    }
  */
  //Play a concert A for half a second
  play(440.0, 0.500);
  delay(500);

  /*
    //Stores the current button value in "previousButtonState" for the next time the program loops through.
    previousButtonState = currentButtonState;
  */
}
