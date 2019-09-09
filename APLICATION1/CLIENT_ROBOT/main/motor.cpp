#include "motor.h"

// LENGHT OF PWM TABLE, ASSIGNATION BETWEEN PWM TO RPM
#define LEN_TABLE_PWM 35

/*start: PROTOTYPES OF PRIVATES FUNCTIONS*/
static void initCounters(char motors);
static void endCounters(char motors);
static void counterPulsesRight(void);
static void counterPulsesLeft(void);
static void rotatePulses(char motors, uint16_t limitPulses);
static int calculationRightPID(int *rpmResult);
static int calculationLeftPID(int *rpmResult);
static int checkRPM(uint8_t rpm);
/*end: PROTOTYPES OF PRIVATES FUNCTIONS*/


//**********INITIALIZATION PULSES VARIABLES **********//
struct confEnconder_t {
  int rightPinB = 3; // (INPUT PIN-INTERRUPT)Signal B of right's motor encoder will be attached to an interrupt.
  int leftPinB  = 2; // (INPUT PIN-INTERRUPT)Signal B of left's motor encoder will be attached to an interrupt.
  uint8_t listPWM_encdB[LEN_TABLE_PWM] =      {0,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,165,175,180,195,200,210,215,220,225,235,240,245,250,255}; //Tabulated and acceptable values for a PWM. For each pwm value in the list, a signal frequency B was obtained from the encoder. This frequency allow us to calculate the corresponding RPM-WHEEL.
  uint8_t listRPMWheel_encdB[LEN_TABLE_PWM] = {0,27,41,48,56,60,69,75,80,86, 89, 96, 99, 105,107,114,115,120,121,125,126,130,134,137,140,141,142,143,145,146,148,149,150,151,157}; // Tabulated RPM-WHEEL for each tabulated PWM value, that is to say, for each tabulated frecuency. Require an initialization.
}  encoder;

struct confMotors_t  {
  int ENA = 5; // control speed of right motor by PWM.
  int ENB = 6; // control speed of left motor by PWM.
  int IN1 = 7; // signal to control the state's direction of right motor.
  int IN2 = 8; // signal to control the state's direction of right motor.
  int IN3 = 9; // signal to control the state's direction of left motor.
  int IN4 = 10;// signal to control the state's direction of left motor.
} pinMotors;


//**********ROTATE PULSES VARIABLES **********//
static int pulsesPowerRotate = POWER_ROTATE;
static uint16_t counterPulsesRM = 0; // Counter of pulses coming from signal B of right's motor.
static uint16_t counterPulsesLM = 0; // Counter of pulses coming from signal B of left's motor.

//**********PID VARIABLES **********//
static char pidMotor;
static float constantProportinal;
static float constantIntegrative;
static float constantDerivative;
static unsigned long sampleTime;
static float sampleTimeSeconds;
static float errorRightSum, errorLeftSum;
static float ITermRight, ITermLeft;
static float timeRPM;
static uint8_t setPointPWM;
static uint8_t setPointRPM;
static int MAXUP=0;
static int MINUP=0;




/*
 *
 *************  INITIALIZATION OF MODULE *************
 *
 */

void initMotor(int ENA, int ENB, int IN1, int IN2, int IN3, int IN4){
  counterPulsesRM = 0;
  counterPulsesLM = 0;
  pinMotors.ENA = ENA;
  pinMotors.ENB = ENB;
  pinMotors.IN1 = IN1;
  pinMotors.IN2 = IN2;
  pinMotors.IN3 = IN3;
  pinMotors.IN4 = IN4;
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void initEnconder(int encodRightPin, int encodLeftPin){
  encoder.rightPinB = encodRightPin;
  encoder.leftPinB  = encodLeftPin;
  pinMode(encoder.rightPinB,INPUT_PULLUP);
  pinMode(encoder.leftPinB,INPUT_PULLUP);
}




/*
 *
 *************  RUN MOTOR AND SET DIRECTION OF ROTATION *************
 *
 */

void setDirectionMotor(char motors, char directionMotor){
  switch (motors) {
    case 'B':
      if (directionMotor == 'F') {
        digitalWrite(pinMotors.IN1, LOW);
        digitalWrite(pinMotors.IN2, HIGH);
        digitalWrite(pinMotors.IN3, LOW);
        digitalWrite(pinMotors.IN4, HIGH);
      } else if (directionMotor == 'B') {
        digitalWrite(pinMotors.IN1, HIGH);
        digitalWrite(pinMotors.IN2, LOW);
        digitalWrite(pinMotors.IN3, HIGH);
        digitalWrite(pinMotors.IN4, LOW);
      } else if (directionMotor == 'S') {
        digitalWrite(pinMotors.IN1, LOW);
        digitalWrite(pinMotors.IN2, LOW);
        digitalWrite(pinMotors.IN3, LOW);
        digitalWrite(pinMotors.IN4, LOW);
        analogWrite(pinMotors.ENA, 0);
        analogWrite(pinMotors.ENB, 0);
      } else if (directionMotor == 'K') {
        analogWrite(pinMotors.ENA, 0);
        analogWrite(pinMotors.ENB, 0);
      }
      break;
    case 'R':
      if (directionMotor == 'F') {
        digitalWrite(pinMotors.IN1, LOW);
        digitalWrite(pinMotors.IN2, HIGH);
      } else if (directionMotor == 'B') {
        digitalWrite(pinMotors.IN1, HIGH);
        digitalWrite(pinMotors.IN2, LOW);
      } else if (directionMotor == 'S') {
        digitalWrite(pinMotors.IN1, LOW);
        digitalWrite(pinMotors.IN2, LOW);
        analogWrite(pinMotors.ENA, 0);
      } else if (directionMotor == 'K') {
        analogWrite(pinMotors.ENA, 0);
      }
      break;
    case 'L':
      if (directionMotor == 'F') {
        digitalWrite(pinMotors.IN3, LOW);
        digitalWrite(pinMotors.IN4, HIGH);
      } else if (directionMotor == 'B') {
        digitalWrite(pinMotors.IN3, HIGH);
        digitalWrite(pinMotors.IN4, LOW);
      } else if (directionMotor == 'S') {
        digitalWrite(pinMotors.IN3, LOW);
        digitalWrite(pinMotors.IN4, LOW);
        analogWrite(pinMotors.ENB, 0);
      } else if (directionMotor == 'K') {
        analogWrite(pinMotors.ENB, 0);
      }
      break;
  }

}

void analogWriteMotor(char motors,int pwm){
  switch (motors) {
      case 'B':
        analogWrite(pinMotors.ENA, pwm);
        analogWrite(pinMotors.ENB, pwm);
        break;
      case 'R':
        analogWrite(pinMotors.ENA, pwm);
        break;
      case 'L':
        analogWrite(pinMotors.ENB, pwm);
        break;
    }
}

/*
* Definition:
*   +If rpm exists in listRPMWheel_encdB table it returns the corresponding pwm signal, otherwise it returns -1.
*/
static int checkRPM(uint8_t rpm){
  for (uint8_t i = 0; i < LEN_TABLE_PWM; i++) {
    if (rpm == encoder.listRPMWheel_encdB[i]) {
      return (int)(encoder.listPWM_encdB[i]);
    }
  }
  return -1;
}

void runMotor(char motors,uint8_t rpmWheel){
  int pwmValue = checkRPM(rpmWheel);
  if (pwmValue != -1) {
    switch (motors) {
      case 'B':
        analogWrite(pinMotors.ENA, pwmValue);
        analogWrite(pinMotors.ENB, pwmValue);
        break;
      case 'R':
        analogWrite(pinMotors.ENA, pwmValue);
        break;
      case 'L':
        analogWrite(pinMotors.ENB, pwmValue);
        break;
    }
  }
}



/*
 *
 *************  COUNTERS TO RPM *************
 *
 */

/*
* Definition:
*   +These function counts the pulses coming from encoder's motors.  These functions are used inside getRPM_Motor or getRPM_BMotor with interruptions.
*/
static void initCounters(char motors){
  switch (motors) {
    case 'B':
      counterPulsesRM = 0;
      counterPulsesLM = 0;
      attachInterrupt(digitalPinToInterrupt(encoder.rightPinB), counterPulsesRight, RISING); //CHANGE,RISING,FALLING
      attachInterrupt(digitalPinToInterrupt(encoder.leftPinB), counterPulsesLeft, RISING); //CHANGE,RISING,FALLING
      break;
    case 'R':
      counterPulsesRM = 0;
      attachInterrupt(digitalPinToInterrupt(encoder.rightPinB), counterPulsesRight, RISING); //CHANGE,RISING,FALLING
      break;
    case 'L':
      counterPulsesLM = 0;
      attachInterrupt(digitalPinToInterrupt(encoder.leftPinB), counterPulsesLeft, RISING); //CHANGE,RISING,FALLING
      break;
  }
}
static void endCounters(char motors){
  switch (motors) {
    case 'B':
      detachInterrupt(digitalPinToInterrupt(encoder.rightPinB));
      detachInterrupt(digitalPinToInterrupt(encoder.leftPinB));
      counterPulsesRM = 0;
      counterPulsesLM = 0;
      break;
    case 'R':
      detachInterrupt(digitalPinToInterrupt(encoder.rightPinB));
      counterPulsesRM = 0;
      break;
    case 'L':
      detachInterrupt(digitalPinToInterrupt(encoder.leftPinB));
      counterPulsesLM = 0;
      break;
  }
}

static void counterPulsesRight(void){
  counterPulsesRM++;
}
static void counterPulsesLeft(void){
  counterPulsesLM++;
}



/*
 *
 *************  ASSING PWM TO RPM *************
 *
 */

void getRPM_BMotor(int pwm,float *rpmWheelR, float *rpmWheelL, unsigned long timeSample){
  //SET UP
  float time = (60.0/(timeSample/1000.0)); //Time Sample in minutes.
  analogWrite(pinMotors.ENA, pwm);
  analogWrite(pinMotors.ENB, pwm);
  initCounters('B');
  unsigned long timeold = millis();
  //LOOP TO CALCULATE RPM-WHEEL
  while (true) {
    if(millis()-timeold >= timeSample){
      noInterrupts();
      *rpmWheelR = (float)(counterPulsesRM*(1.0/RESOLUTION_ROUND)*(1.0/GEAR_RATIO)*time);
      *rpmWheelL = (float)(counterPulsesLM*(1.0/RESOLUTION_ROUND)*(1.0/GEAR_RATIO)*time);
      //setDirectionMotor('B','S');
      interrupts();
      break;  // to get out of while after sample time
    }
  }
  endCounters('B');
}

void getRPM_Motor(int pwm,float *rpmWheel, char motor, unsigned long timeSample){
  //SET UP
  float time = (60.0/(timeSample/1000.0)); //Time Sample in minutes.
  if (motor == 'R') {
    analogWrite(pinMotors.ENA, pwm);
  } else if (motor == 'L') {
    analogWrite(pinMotors.ENB, pwm);
  }
  initCounters(motor);
  unsigned long timeold = millis();
  //LOOP TO CALCULATE RPM-WHEEL
  while (true) {
    if(millis()-timeold >= timeSample){
      noInterrupts();
      if (motor == 'R') {
        *rpmWheel = (float)(counterPulsesRM*(1.0/RESOLUTION_ROUND)*(1.0/GEAR_RATIO)*time);
      } else if (motor == 'L') {
        *rpmWheel = (float)(counterPulsesLM*(1.0/RESOLUTION_ROUND)*(1.0/GEAR_RATIO)*time);
      }
      //setDirectionMotor(motor,'S');
      interrupts();
      break; // to get out of while after sample time
    }
  }
  endCounters(motor);
}



/*
 *
 *************  ROTATE MOTOR *************
 *
 */

/*
* Definition:
*   + This function sets a speed for both right and left motor, only for right motor or only for left motor, and stop the motors when the counterPulses arrives to limitPulses.
*/
static void rotatePulses(char motors, uint16_t limitPulses){
  bool flag = true;
  initCounters(motors);
  switch (motors) {
    case 'B':
      analogWrite(pinMotors.ENA, pulsesPowerRotate);
      analogWrite(pinMotors.ENB, pulsesPowerRotate);
      break;
    case 'R':
      analogWrite(pinMotors.ENA, pulsesPowerRotate);
      break;
    case 'L':
      analogWrite(pinMotors.ENB, pulsesPowerRotate);
      break;
  }
  while (flag) {
    noInterrupts();
    switch (motors) {
      case 'B':
        if ((counterPulsesRM >= limitPulses) && (counterPulsesLM >= limitPulses)) {
          setDirectionMotor('B','K');
          flag = false;
        }
        break;
      case 'R':
        noInterrupts();
        if (counterPulsesRM >= limitPulses) {
          setDirectionMotor('R','K');
          flag = false;
        }
        interrupts();
        break;
      case 'L':
        if (counterPulsesLM >= limitPulses) {
          setDirectionMotor('L','K');
          flag = false;
        }
        break;
    }
    interrupts();
  }
  endCounters(motors);
}

void powerRotationMotor(uint8_t power){
  pulsesPowerRotate = power;
}

void rotateWheelMotor(char motors, uint16_t degrees){
  if(degrees <= 360 && (motors == 'R' || motors == 'L' || motors == 'B' )){
    int pulses = (int)GET_PULSES_WHEEL((float)degrees);
    rotatePulses(motors,pulses);
  }
}

void rotateCarMotor(char direction, uint16_t degrees){
  uint8_t times90degres = degrees/90;
  char directionRight;
  char directionLeft;
  char motorBackward;
  char motorForward;
  if (direction == 'R') {
    directionRight = 'B';
    directionLeft  = 'F';
    motorBackward  = 'R';
    motorForward   = 'L';

  }
  else if (direction == 'L'){
    directionRight = 'F';
    directionLeft  = 'B';
    motorBackward  = 'L';
    motorForward   = 'R';
  }
  for (uint8_t i = 1; i <= times90degres; i++) {
    // ROTATE 90 DEGREES X TIMES == times90degres.
    // Due to a radius of 17.5, we need to advance 13 centimeters in order to rotate 45 degrees de car.
    // The car rotates 45 degrees in backward mode and 45 degrees in forward mode with the objective to use the minimun space.
    for(int i= 0 ; i<12;i++){
      setDirectionMotor('R', directionRight);
      setDirectionMotor('L', directionLeft);
      moveCentimetersMotor(motorBackward,1);
      delay(300);
    }
    for(int i= 0 ; i<12;i++){
      setDirectionMotor('R', directionRight);
      setDirectionMotor('L', directionLeft);
      moveCentimetersMotor(motorForward,1);
      delay(300);
    }
    delay(300);
  }
}

void moveCentimetersMotor(char motors, uint8_t centimeters){
  if(motors == 'R' || motors == 'L' || motors == 'B' ){
    uint16_t pulses = GET_PULSES_MOVE(centimeters);
    rotatePulses(motors,pulses);
  }
}



/*
 *
 *************  PID MOTOR *************
 *
 */

 void initPIDMotor(char motor,uint8_t spRPM, float Kp, float Ki, float Kd, unsigned long timeSample){
   setPointPWM = checkRPM(spRPM);
   if (setPointPWM != -1) {
     setPointRPM = spRPM;
     MAXUP = setPointPWM/2;
     MINUP = -setPointPWM/2;
     pidMotor = motor;
     sampleTime = timeSample;
     sampleTimeSeconds = sampleTime/1000.0;
     constantProportinal = Kp;
     constantIntegrative = Ki*sampleTimeSeconds;
     constantDerivative = Kd/sampleTimeSeconds;

     errorLeftSum = 0;
     errorRightSum = 0;
     timeRPM = (60.0/(timeSample/1000.0)); //Time Sample in minutes.
     //SET UP
     if (pidMotor == 'B'){
       analogWrite(pinMotors.ENA, setPointPWM);
       analogWrite(pinMotors.ENB, setPointPWM);
     } else  if (pidMotor == 'R') {
       analogWrite(pinMotors.ENA, setPointPWM);
     } else if (pidMotor == 'L') {
       analogWrite(pinMotors.ENB, setPointPWM);
     }
     initCounters(motor);
   }
 }

static  int calculationRightPID(int *rpmResult){
    uint8_t rpmWheelRight;
    float errorRight, upRight, errorRightDerivative; //may be double
    static float PreviousErrorRight; //may be double
    //CALCULATION OF RPM-WHEEL
    rpmWheelRight = (uint8_t)(counterPulsesRM*(1.0/RESOLUTION_ROUND)*(1.0/GEAR_RATIO)*timeRPM);
    * rpmResult = rpmWheelRight;

    counterPulsesRM = 0;
    // APPLY - CALCULATION CONTROLLER PID
    //********* ERROR SETPOINT *********
    errorRight  = setPointRPM - rpmWheelRight;
    //if (errorRight<0) errorRight= -errorRight;

    //********* INTEGRATIVE *********
    ITermRight += constantIntegrative*errorRight;
    if(ITermRight> setPointPWM ) ITermRight = setPointPWM;
    else if(ITermRight < -setPointPWM ) ITermRight = -setPointPWM;

    //********* DERIVATIVE *********
    errorRightDerivative = errorRight - PreviousErrorRight;
    PreviousErrorRight = errorRight;

    //*************** OUTPUT CONTROLLER PID: WE OBTEIN PWM SIGNAL***************************
    upRight  = constantProportinal*errorRight + ITermRight - constantDerivative*errorRightDerivative;

    if (upRight > (setPointPWM + MAXUP)){      upRight = setPointPWM  + MAXUP;}
    else if (upRight < (-setPointPWM + MINUP)){upRight = -setPointPWM + MINUP;}

    if (upRight > 255){  upRight = 255;}
    else if (upRight < 0){ upRight = 0;}

    return upRight;
  }



static int calculationLeftPID(int *rpmResult){
    uint8_t rpmWheelLeft;
    float errorLeft, upLeft, errorLeftDerivative;//may be double
    static float PreviousErrorLeft;//may be double
    //CALCULATION OF RPM-WHEEL
    rpmWheelLeft  = (uint8_t)(counterPulsesLM*(1.0/RESOLUTION_ROUND)*(1.0/GEAR_RATIO)*timeRPM);
    *rpmResult = rpmWheelLeft;

    counterPulsesLM = 0;
    // APPLY - CALCULATION CONTROLLER PID
    //********* ERROR SETPOINT *********
    errorLeft   = setPointRPM - rpmWheelLeft;
    //if (errorLeft<0) errorLeft = -errorLeft;

    //********* INTEGRATIVE *********
    ITermLeft += constantIntegrative*errorLeft;
    if(ITermLeft> setPointPWM) ITermLeft = setPointPWM;
    else if(ITermLeft < -setPointPWM) ITermLeft = -setPointPWM;

    //********* DERIVATIVE *********
    errorLeftDerivative = errorLeft - PreviousErrorLeft;
    PreviousErrorLeft = errorLeft;

    //*************** OUTPUT CONTROLLER PID: WE OBTEIN PWM SIGNAL***************************
    upLeft   = constantProportinal*errorLeft + ITermLeft - constantDerivative*errorLeftDerivative;

    if (upLeft > (setPointPWM + MAXUP)){       upLeft  = setPointPWM + MAXUP;}
    else if (upLeft < (-setPointPWM + MINUP)){ upLeft  = -setPointPWM + MINUP;}

    if (upLeft > 255){  upLeft = 255;}
    else if (upLeft < 0){ upLeft = 0;}


    return upLeft;
  }

 void execute2PIDMotor(int *rightRPM, int *leftRPM){
   int rightPWM;
   int leftPWM;
   unsigned long timeold = millis();
   //LOOP TO CALCULATE RPM-WHEEL
   while (true) {
     if(millis()-timeold >= sampleTime){
       noInterrupts();
       if (pidMotor == 'B') {
         leftPWM  = calculationLeftPID(leftRPM);
         rightPWM = calculationRightPID(rightRPM);
         analogWrite(pinMotors.ENA, rightPWM);
         analogWrite(pinMotors.ENB, leftPWM);
       } else if (pidMotor == 'R') {
         rightPWM = calculationRightPID(rightRPM);
         analogWrite(pinMotors.ENA, rightPWM);
       } else if (pidMotor == 'L') {
         leftPWM = calculationLeftPID(leftRPM);
         analogWrite(pinMotors.ENB, leftPWM);
       }
       timeold = millis();
       interrupts();
       break;
     }
   }
 }

 void executePIDMotor(int *valueRPM){
   int rightPWM;
   int leftPWM;
   unsigned long timeold = millis();
   //LOOP TO CALCULATE RPM-WHEEL
   while (true) {
     if(millis()-timeold >= sampleTime){
       noInterrupts();
       if (pidMotor == 'R') {
         rightPWM = calculationRightPID(valueRPM);
         analogWrite(pinMotors.ENA, rightPWM);
       } else if (pidMotor == 'L') {
         leftPWM = calculationLeftPID(valueRPM);
         analogWrite(pinMotors.ENB, leftPWM);
       }
       timeold = millis();
       interrupts();
       break;
     }
   }
 }


 void endPIDMotor(void){
   errorLeftSum = 0;
   errorRightSum = 0;
   setDirectionMotor(pidMotor,'S');
   endCounters(pidMotor);
 }
