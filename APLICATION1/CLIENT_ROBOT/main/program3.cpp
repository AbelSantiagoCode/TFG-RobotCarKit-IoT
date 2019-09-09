#include "program3.h"
#include "motor.h"
#include "ultrasound.h"
#include "stringlib.h"
#include <Servo.h>
#define LLINDAR_DISTANCE 5
#define LLINDAR2_DISTANCE 25

Servo myservo;

// GLOBAL VARIABLES
typedef enum state_t {STOP, START };

//PROTOTYPES
void getDegreeDistance(int *degree, int *distance);
void moveTrajectory(int *degree, int *distance);




void getDegreeDistance(int *degree, int *distance){
  int minDistance;
  int minDegrees;
  int currentDistance;
  int degreesT[4] = {45,90,135,180};

  myservo.write(0);
  delay(2000);
  minDistance = distanceCMUltSound();
  minDegrees = 0;

  for (int i = 0; i < 4; i++) {

    myservo.write(degreesT[i]);
    delay(2000);
    currentDistance = distanceCMUltSound();

    if (currentDistance <= minDistance) {
      minDistance = currentDistance;
      minDegrees = degreesT[i];
    }
  }
  myservo.write(90);
  delay(2000);
  *degree = minDegrees;
  *distance = minDistance;
}

void moveTrajectory(int *degree, int *distance){
  bool flag = true;
  int currentDistance;

  while (flag) {
    if (*degree == 90) {
      flag = false;
      setDirectionMotor('B', 'F');
      moveCentimetersMotor('B', *distance);
    }
    else if (*degree <= 90) { // ROTATION RIGHT
      setDirectionMotor('R', 'B');
      moveCentimetersMotor('R', 3);
      setDirectionMotor('L', 'F');
      moveCentimetersMotor('L', 3);
      currentDistance = distanceCMUltSound();
      delay(800);
      if (currentDistance <= *distance) {
        flag = false;
        setDirectionMotor('B', 'F');
        moveCentimetersMotor('B', currentDistance);
      }
    }
    else if (*degree > 90) {// ROTATION LEFT
      setDirectionMotor('L', 'B');
      moveCentimetersMotor('L', 2);
      setDirectionMotor('R', 'F');
      moveCentimetersMotor('R', 2);
      currentDistance = distanceCMUltSound();
      delay(800);
      if (currentDistance <= *distance) {
        flag = false;
        setDirectionMotor('B', 'F');
        moveCentimetersMotor('B', currentDistance);
      }
    }
  }
}

void initProgram3(void){
  initMotor(5,10,7,8,11,12);
  initEnconder(0,1);
  setDirectionMotor('B', 'S');
  initUltSound(3,4); // I NEED TO CHECK THE PINS
  myservo.attach(14); // I NEED TO CHECK THE PINS
  powerRotationMotor(150);
}

void FSM_PRG3(bool event, char message[], bool *FlagPSelected, int *degree, int *distance){
  static state_t state = STOP;
  int currentDistance;
  int currentDegree;

  switch (state) {
    case STOP:
      if (event == true) {
        if (beginWithString(message,"START")) {
          state = START;
        }
        else if (beginWithString(message,"EXIT")) {
          setDirectionMotor('B', 'S');
          *FlagPSelected = false;
        }
      }
      break;
    case START:
      if (event == true) {
        if (beginWithString(message,"STOP")) {
          state = STOP;
        }
        else if (beginWithString(message,"EXIT")) {
          state = STOP;
          setDirectionMotor('B', 'S');
          *FlagPSelected = false;
        }
      } else {
        // GET MINIMUN CURRENT DEGREE AND DISTANCE
        getDegreeDistance(&currentDegree, &currentDistance);
        // CHECK IF DISTANCE IS GREATER THAN THRESHOLD, APPLY ROTATION AND ADVANCE
        if (currentDistance > LLINDAR_DISTANCE && currentDistance <= LLINDAR2_DISTANCE) {
          moveTrajectory(&currentDegree, &currentDistance);
          // SAVE DEGREE AND DISTANCE ONLY WHEN CURRENTDISTANCE IS GREATER THAN THRESHOLD
          *degree = currentDegree;
          *distance = currentDistance;
        }
      }
      break;
  }
}
