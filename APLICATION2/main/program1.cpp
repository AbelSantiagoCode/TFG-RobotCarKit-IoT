#include "motor.h"
#include "stringlib.h"
#include "program1.h"

// GLOBAL VARIABLES
typedef enum state_t {STOP, MOTOR_PID };


// PROTOTYPES
static int getValue(char message[]);

// ************+ CALL TO INIT-PROGRAMS *******************//

void initProgram1(void){
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
  // Serial.print("GET VALUE: ");
  // Serial.println(atoi(message));
  return  atoi(message);
}



void FSM_PRG1(bool event, char message[], bool *FlagPSelected){
  static state_t state = STOP;
  static int rightRPM;
  static int leftRPM;
  switch (state) {
    case STOP:
      if (event == true) {
        //Serial.println("STATE: STOP");
        if (beginWithString(message,"FORWARD_PID,")) {
          //Serial.println("FORWARD_PID");
          state = MOTOR_PID;
          setDirectionMotor('B', 'F');
          uint8_t setPoint = getValue(message);
          initPIDMotor('B', setPoint, 1.8, 0.1, 0.0,700);
        }
        else if (beginWithString(message,"BACKWARD_PID,")) {
          //Serial.println("BACKWARD_PID");
          state = MOTOR_PID;
          setDirectionMotor('B', 'B');
          uint8_t setPoint = getValue(message);
          initPIDMotor('B', setPoint, 1.8, 0.1, 0.0,700);
        }
        else if (beginWithString(message,"POWER_ROTATION,")) {
          //Serial.println("POWER_ROTATION");
          uint8_t power = getValue(message);
          powerRotationMotor( power);
        }
        else if (beginWithString(message,"STOP")) {
          //Serial.println("STOP");
          setDirectionMotor('B', 'S');
        }
        else if (beginWithString(message,"ROTATE,")) {
          //Serial.println("ROTATE");
          shiftLeftString(message, lenString("ROTATE,"));
          if (beginWithString(message,"R")) {
            setDirectionMotor('L', 'F');
            moveCentimetersMotor('L', 1);
          } else if (beginWithString(message,"L")) {
            setDirectionMotor('R', 'F');
            moveCentimetersMotor('R', 1);
          }
        }
        else if (beginWithString(message,"ROTATE_CAR,")) {
          //Serial.println("ROTATE_CAR");
          shiftLeftString(message, lenString("ROTATE_CAR,"));
          if (beginWithString(message,"R,")) {
            uint16_t degrees = getValue(message); // shift until ',' and return the value int
            rotateCarMotor('R', degrees);
          } else if (beginWithString(message,"L,")) {
            uint16_t degrees = getValue(message);
            rotateCarMotor('L', degrees);
          }
        }
        else if (beginWithString(message,"FORWARD_CM,")) {
          //Serial.println("FORWARD_CM");
          setDirectionMotor('B', 'F');
          uint8_t centimeters = getValue(message);
          moveCentimetersMotor('B', centimeters);
        }
        else if (beginWithString(message,"BACKWARD_CM,")) {
          //Serial.println("BACKWARD_CM");
          setDirectionMotor('B', 'B');
          uint8_t centimeters = getValue(message);
          moveCentimetersMotor('B', centimeters);
        }
        else if (beginWithString(message,"EXIT")) {
          //Serial.println("EXIT");
          setDirectionMotor('B', 'S');
          *FlagPSelected = false;
        }
      }
      break;
    case MOTOR_PID:
      if (event == true) {
        //Serial.println("STATE: MOTOR_PID");
        if (beginWithString(message,"FORWARD_PID,")) {
          //Serial.println("FORWARD_PID");
          setDirectionMotor('B', 'F');
          uint8_t setPoint = getValue(message);
          initPIDMotor('B', setPoint, 1.8, 0.1, 0.0,700);
        }
        else if (beginWithString(message,"BACKWARD_PID,")) {
          //Serial.println("BACKWARD_PID");
          setDirectionMotor('B', 'B');
          uint8_t setPoint = getValue(message);
          initPIDMotor('B', setPoint, 1.8, 0.1, 0.0,700);
        }
        else if (beginWithString(message,"POWER_ROTATION,")) {
          //Serial.println("POWER_ROTATION");
          uint8_t power = getValue(message);
          powerRotationMotor( power);
        }
        else if (beginWithString(message,"STOP")) {
          //Serial.println("STOP");
          state = STOP;
          endPIDMotor();
          setDirectionMotor('B', 'S');
        }
        else if (beginWithString(message,"ROTATE,")) {
          //Serial.println("ROTATE");
          state = STOP;
          endPIDMotor();
          shiftLeftString(message, lenString("ROTATE,"));
          if (beginWithString(message,"R")) {
            setDirectionMotor('L', 'F');
            moveCentimetersMotor('L', 1);
          } else if (beginWithString(message,"L")) {
            setDirectionMotor('R', 'F');
            moveCentimetersMotor('R', 1);
          }
        }
        else if (beginWithString(message,"ROTATE_CAR,")) {
          //Serial.println("ROTATE_CAR");
          state = STOP;
          endPIDMotor();
          shiftLeftString(message, lenString("ROTATE_CAR,"));
          if (beginWithString(message,"R,")) {
            uint16_t degrees = getValue(message); // shift until ',' and return the value int
            rotateCarMotor('R', degrees);
          } else if (beginWithString(message,"L,")) {
            uint16_t degrees = getValue(message);
            rotateCarMotor('L', degrees);
          }
        }
        else if (beginWithString(message,"FORWARD_CM,")) {
          //Serial.println("FORWARD_CM");
          state = STOP;
          endPIDMotor();
          setDirectionMotor('B', 'F');
          uint8_t centimeters = getValue(message); // shift until ',' and return the value int
          moveCentimetersMotor('B', centimeters);
        }
        else if (beginWithString(message,"BACKWARD_CM,")) {
          //Serial.println("BACKWARD_CM");
          state = STOP;
          endPIDMotor();
          setDirectionMotor('B', 'B');
          uint8_t centimeters = getValue(message); // shift until ',' and return the value int
          moveCentimetersMotor('B', centimeters);
        }
        else if (beginWithString(message,"EXIT")) {
          //Serial.println("EXIT");
          state = STOP;
          endPIDMotor();
          setDirectionMotor('B', 'S');
          *FlagPSelected = false;
        }
      } else {
        execute2PIDMotor(&rightRPM,&leftRPM);
      }
      break;
  }
}
