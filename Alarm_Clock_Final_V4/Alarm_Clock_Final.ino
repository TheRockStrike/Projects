/*
  Kenny Akers
  Mr. Paige
  Intro to CS
  5/25/16

                                             Alarm Clock Final Project

  The alarm clock. This program utilizes the millis() function as its base time keeping ability. From there,
  several function methods convert the time since the program started running into seconds, minutes, and hours.
  The time can be set by pushing two buttons: the increment hour button (Green) and the increment minute button (Blue).
  Time is displayed in 24 format and allows an alarm to be set with the same incriment hour and minute buttons.
  When the time reaches the set alarm time, a speaker will sound and a LED illuminated on and off once each second.
  Additional functions include: AM/PM display, Enable/Disable switch, and a snooze button, which will disable the sounding
  alarm for five minutes.

*/

//Libraries
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h> // For the LCD Display

//Button declarations.
const int incrementHourButton = 5;
const int incrementMinuteButton = 6;
const int snoozeButton = 7;

//Switch declarations.
const int setAlarmSwitch = 8;
const int alarmOnOffSwitch = 9;

//Output pin declarations.
const int led = 10;
const int speaker = 11;
const int alarmGateControl = 12;

//Begin by setting the alarm to 0.
int alarmHourVal = 0;
int alarmMinuteVal = 0;

//Variable to store the state of the alarm in. For use by the snooze button.
//Initially set to false as the alarm is not going off.
boolean alarmState = false;

//Variable to store the time (in milliseconds) since the Arduino started running.
//Initially set to 0.
long timeSinceArduinoStartTimeInMillisSinceMidnight = 0;

Adafruit_LiquidCrystal lcd(0); //Connect the LCD via i2c

void setup() {
  // Initialize Serial and LCD
  Serial.begin(9600);
  lcd.begin(16, 2);

  // For loop to initalize the input pins
  for (int pin = 5; pin <= 9; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }
  // For loop to initalize the output pins
  for (int pin = 10; pin <= 12; pin++) {
    pinMode(pin, OUTPUT);
  }
}

// Function method to return the number of seconds that have passed since
// the program started running.
long seconds() {

  int seconds = currentTimeInMillisSinceMidnight() / 1000; // Milliseconds (returned by the currentTimeInMIllisSinceMidnight method) --> seconds conversion.
  if (seconds >= 0 && seconds <= 59) { // If statement to return the number of seconds within the given range.
    return seconds;
  }
  else { // If the number of seconds exceeds the above range, which is will after one minute, 'restart' the counter by returning the remainder of SECONDS / 60.
    return seconds % 60;
  }
}

// Function method to return the number of minutes that have passed since the
// program started running.
int minutes() {

  int minutes = (currentTimeInMillisSinceMidnight() / 1000) / 60; // Milliseconds --> minutes conversion.
  if (minutes >= 0 && minutes <= 59) { // If statement to return the number of seconds within the given range.
    return minutes;
  }
  else { // If the number of minutes exceeds the above range, 'restart' the counter by returning the remainder of MINUTES / 60.
    return minutes % 60;
  }
}

// Function method to return the number of hours that have passed since the
// program started running.
int hours() {

  int hours = ((currentTimeInMillisSinceMidnight() / 1000) / 60) / 60; // Milliseconds --> hours conversion.
  if (hours >= 0 && hours <= 23) { // If statement to return the number of hours in 24 hour format.
    return hours;
  }
  else { // Once the number of hours exceeds the range of the if statement, 'restart' it by returning the remainder of HOURS / 60.
    return hours % 24;
  }
}

// Simple function method to return the number of milliseconds since the Arduino started running.
long currentTimeInMillisSinceMidnight() {
  // Return the time with the millis() counter value added to it.
  return timeSinceArduinoStartTimeInMillisSinceMidnight + millis();
}

// Method to display the time on the LCD Display.
void displayTime() {

  // Variables to store the values returned by the corresponding function methods above.
  int secondVal = seconds();
  int minuteVal = minutes();
  int hourVal = hours();

  // If the number of hours exceeds 11 (12 and above), display PM on the LCD.
  if (hourVal >= 12) {
    lcd.setCursor(13, 0);
    lcd.print(" PM");
  }
  else { // Otherwise, AM will be displayed.
    lcd.setCursor(13, 0);
    lcd.print(" AM");
  }

  // Always print on the first row, Time:
  lcd.setCursor(0, 0);
  lcd.print("Time:");

  //  \ / \ / \ / \ / \ / \ / \ / \ / \ / \ / \ / \ / \ / Below are formating If statements for the LCD display.
  if (secondVal < 10) {
    lcd.setCursor(6, 0);
    lcd.print(hourVal);
    lcd.print(":");
    lcd.print(minuteVal);
    lcd.print(":0");
    lcd.print(secondVal);
  }
  else {
    lcd.setCursor(6, 0);
    lcd.print(hourVal);
    lcd.print(":");
    lcd.print(minuteVal);
    lcd.print(":");
    lcd.print(secondVal);
  }

  if (hourVal >= 10) {
    lcd.setCursor(5, 0);
    lcd.print(hourVal);
    lcd.print(":");
    lcd.print(minuteVal);
    lcd.print(":");
    lcd.print(secondVal);
  }
  else if (hourVal < 10) {
    lcd.setCursor(5, 0);
    lcd.print(" ");
    lcd.print(hourVal);
    lcd.print(":");
    lcd.print(minuteVal);
    lcd.print(":");
    lcd.print(secondVal);
  }
  else {
  }

}

// Method to set the alarm when called by the while statement in the loop below.
void setAlarm() {

  //Constantly display the "Set Alarm" screen, which will display the current alarm.
  lcd.setCursor(0, 0);
  lcd.print("Set Alarm: ");
  lcd.setCursor(10, 0);
  lcd.print(alarmHourVal);
  lcd.print(": ");
  lcd.print(alarmMinuteVal);
  lcd.print(" ");

  //If the incrimentHourButton is pressed, incriment the alarmHourVal and store that new value in the alarm function.
  if (digitalRead(incrementHourButton) == LOW) {
    alarmHourVal++;
  }
  //If the incrimentMinuteButton is pressed, incriment the alarmMinuteVal and store that new value in the alarm function.
  else if (digitalRead(incrementMinuteButton) == LOW) {
    alarmMinuteVal++;
  }
  else {}

  //If the alarmHourVal exceeds 12, reset it to 1.
  if (alarmHourVal > 23) {
    alarmHourVal = 0;
  }
  //If the alarmMinuteVal exceeds 60, reset it to 0.
  else if (alarmMinuteVal > 59) {
    alarmMinuteVal = 0;
  }
  else {}

}

void alarm() {
  digitalWrite(led, HIGH);
  tone(speaker, 440);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);

  digitalWrite(alarmGateControl, HIGH);
}


void loop() {
  // Variables to store the values returned by the corresponding function methods above.
  int secondVal = seconds();
  int minuteVal = minutes();
  int hourVal = hours();

  // While the setAlarmSwitch is flipped up, execute the code within it.
  while (digitalRead(setAlarmSwitch) == HIGH) {

    setAlarm(); // Call the setAlarm function.
    for (int i = 0; i <= 1; i++) { // For loop to only print the following once.
      lcd.setCursor(0, 1);
      lcd.print("Alarm: ON      ");
    }
  }

  // If the alarm Enable/Disable switch is not flipped up, print "Alarm: Disabled" on the LCD.
  if (digitalRead(alarmOnOffSwitch) == LOW) {
    lcd.setCursor(0, 1);
    lcd.print("Alarm: Disabled");
  }
  // If the alarm Enable/Disable switch is flipped up, print "Alarm: Enabled" (with a few extra spaces to clear the display) on the LCD.
  else if (digitalRead(alarmOnOffSwitch) == HIGH) {
    lcd.setCursor(0, 1);
    lcd.print("Alarm: Enabled  ");
  }

  // If the increment minute button is pressed, add 60,000 milliseconds (the number of milliseconds in a minute)
  // to the timer since the Arduino started running.
  if (digitalRead(incrementMinuteButton) == LOW) {
    timeSinceArduinoStartTimeInMillisSinceMidnight += 60000;
  }
  // If the increment hour button is pressed, add 3,600,000 milliseconds (the number of milliseconds in an hour) to the timer.
  else if (digitalRead(incrementHourButton) == LOW) {
    timeSinceArduinoStartTimeInMillisSinceMidnight += 3600000;
  }
  // Alarm function: Becomes true when both the minute and hour values become equal to the minute and hour values set as the alarm.
  if (alarmHourVal == hourVal && alarmMinuteVal == minuteVal) {

    if (digitalRead(alarmOnOffSwitch) == HIGH) { // Checks if the Enable/Disable switch is flipped up (Enabled).
      alarm(); // Calls the alarm() method.
      alarmState = true; // Sets the alarmState to be TRUE, i.e. the alarm is going off.

    }
  }
  else {
    alarmState = false; // Otherwise, the alarm is not going off, therefore alarmState is FALSE.
  }

  // If the snooze button is pressed AND the alarm is going off, do the following.
  if (digitalRead(snoozeButton) == LOW && alarmState == true) {

    for (int i = 1; i <= 300; i++) { // For loop that will execute for 5 minutes (1 second cycle time * 300 seconds in 5 minutes = 5 minutes).
      displayTime(); // Still call the displayTime method.
      digitalWrite(alarmGateControl, LOW); // Disable the alarm.
      delay(1000); // Delay a second (for timing).
    }
  }

  displayTime(); // Always call the displayTime method
  digitalWrite(alarmGateControl, LOW); // Disable the speaker by default.
}


