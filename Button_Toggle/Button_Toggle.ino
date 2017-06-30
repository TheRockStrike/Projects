int inPin = 5;         // the number of the input pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long pinTime = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{
  Serial.begin(9600);
  pinMode(inPin, INPUT);
}

void loop()
{
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - pinTime > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    pinTime = millis();    
  }

  previous = reading;
  Serial.println(state);
}

