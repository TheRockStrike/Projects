/*
  Kenny Akers and Gabe Cordova
  Mr. Paige
  Intro to CS
  5/13/16

  This lab uses a 10-segment LED bar IC and a potentiometer to control a 10-slot boolean array. This array
  in turn controls each of the LEDs in accordance with values from the potentiometer.
*/
// Declares the array "bars" as a boolean array with 10 slots.
boolean bars[10];

void setup() {
  // For loop to initialize pins 2-11 as outputs for the LED bar display
  for (int pin = 2; pin < 12; pin++) {
    pinMode(pin, OUTPUT);
  }
  // Declares the A0 analog in to be an input.
  pinMode(A0, INPUT);

  // Serial communication for debugging - printing out values.
  Serial.begin(9600);
}
// This method does most the visual work by updating the display (lighting up more or less LEDs).
void updateBarGraph(boolean bars[]) {
  for (int i = 0; i < 10; i++) { //For loop for each slot in the array.
    if (bars[i] == true) { // If the slot has a TRUE in it...
      digitalWrite(i + 2, HIGH); // Turn on the corresponding LED.
    }
    else if (bars[i] == false) { // If the slot has a FALSE in it...
      digitalWrite(i + 2, LOW); // Turn off the corresponding LED.
    }
  }
}
// This method clears the display by storing a FALSE in each slot in the array.
void clearmethod(boolean bars[]) {
  for (int i = 0; i < 10; i++) { // For loop for each slot.
    bars[i] = false; // Stores a FALSE in each slot.
  }
}
// This method lights up the entire display by storing a TRUE in each slot in the array.
void fill(boolean bars[], int k) {
  for (int i = 0; i < k; i++) { // For loop for each slot.
    bars[i] = true; // Set each slot to TRUE

  }
  Serial.println(k); //  Print out the value stored in k for debugging.
}

void loop() {
  int signalValue = analogRead(A0); // Store an analog reading of the potentiometer in "signalValue."

  clearmethod(bars); // Clear the display.
  fill(bars, (signalValue / 1024.0) * 10); // Fill the display with the corresponding value from the potentiometer.

  updateBarGraph(bars); // Then update the display depending on the potentiometer reading.

}
