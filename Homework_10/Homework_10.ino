/*
  Kenny Akers
  Mr. Paige
  Intro to CS
  5/11/16

  This homework serves as practice for working with an array filled with letter grades. Five examples are shown below, each executing a different function.
*/

char grades[6] = {'B', 'D', 'A', 'C', 'A', 'A'}; //Declares an array, "grades," to hold 6 chars of B, D, A, C, A, and A.

void setup() {
  //Start a serial connection
  Serial.begin(9600);

  //Set all slots in grades to blanks
  initialize(grades, 6);
}

// #1
void initialize(char grades[], int n) { //Method to initialize the array "grades," with n number of memory slots.
  // Store a blank in each slot of the array
  for (int i = 1; i <= n; i++) { //For every slot, store a space in there
    grades[i] = ' ';
  }
}

// #2
int countNumberOfAs(char grades[]) { // This method counts the number of A's in the array.
  int count = 0;
  for (int i = 0; i < 6; i++) {
    if (grades[i] == 'A') { //If there is an A in each slot...
      count++; //Increment count
    }
  }
  return count; //Return the number of A's in the array
}
// #3
int countNumberOfACertainGrade(char grades[], char grade) { //This method counts the number of a specific grade.
  int count = 0;
  for (int i = 0; i < 6; i++) {
    if (grades[i] == grade) { //If there is a certain letter grade in that slot...
      count++; //Increment count
    }
  }
  return count; //Return the number of that grade in the array.
}
// #4
int countNumberOfNonBlankGrades(char grades[]) { //This method counts the number of non-blank grades.
  int count = 0;
  for (int i = 0; i < 6; i++) {
    if (grades[i] != ' ') { //Checks if there is a blank in each slot. If there is, the condition is false and count is not incremented.
      //If a blank is not in that slot, i.e., there is a letter grade in there. count is incremented.
      count++; //increment count
    }
    return count; //Return the number of grades
  }
}
// #5
void appendNewGrade(char grades[], char newGrade) { //This method appends the list of grades with a new grade (newGrade).
  int slotNumber = countNumberOfNonBlankGrades(grades); //Stores the number of blanks in the array in the variable "slotNumber."
  for (int i = 0; i <= slotNumber; i++) { //Runs the amount of times according to the number of empty slots in the array.
    if (grades[slotNumber] == ' ') { //If that slot number has nothing in it (verification step)
      grades[slotNumber] = newGrade; //Store the specified newGrade in that slot.
    }
  }
}

void loop() {

  //Serial.println(countNumberOfAs(grades));
  //Serial.println(countNumberOfACertainGrade(grades, 'C'));
  delay(750);

}
