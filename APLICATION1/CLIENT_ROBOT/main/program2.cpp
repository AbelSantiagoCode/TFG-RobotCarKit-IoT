#include "motor.h"
#include "stringlib.h"
#include "program2.h"

// GLOBAL VARIABLES
typedef enum state_t {STOP, START };

// PROTOTYPES
static int getValue(char message[]);

// ************+ CALL TO INIT-PROGRAMS *******************//

void initProgram4(void){
  initMotor(5,10,7,8,11,12);
  initEnconder(0,1);
  setDirectionMotor('B', 'S');
}

// CODE OF PROGRAMS.H ONLY FOR DEBUG
static int getValue(char message[]) {
  while (message[0] != ',') {
    shiftLeftString(message, 1);
  }
  shiftLeftString(message, 1);
  return  atoi(message);
}

void FSM_PRG4(bool event, char message[], int *rightRPM, int *leftRPM , bool *FlagPSelected){
  static state_t state = STOP;
  switch (state) {
    case STOP:
      if (event == true) {
        if (beginWithString(message,"START,")) {
          state = START;
          setDirectionMotor('B', 'F');
          uint8_t setPoint = getValue(message);
          initPIDMotor('B', setPoint, 1.1, 0.3, 0.0,2000);
        }
        else if (beginWithString(message,"EXIT")) {
          *FlagPSelected = false;
        }
      }
      break;
    case START:
      if (event == true) {
        if (beginWithString(message,"STOP")) {
          state = STOP;
          endPIDMotor();
          setDirectionMotor('B', 'S');
        }

        else if (beginWithString(message,"EXIT")) {
          state = STOP;
          endPIDMotor();
          setDirectionMotor('B', 'S');
          *FlagPSelected = false;
        }
      } else {
        execute2PIDMotor(rightRPM,leftRPM);
      }
      break;
  }
}
