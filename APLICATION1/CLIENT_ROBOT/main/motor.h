#ifndef MOTOR
#define MOTOR

#include <stdint.h>
#import <Arduino.h>

#define GEAR_RATIO 120.0
#define RESOLUTION_ROUND 8.0 // Resolution of 16 pulse single per round with the tow signals A and B.



#define POWER_ROTATE  127  // may be 130
#define ROUND_WHEEL360   960.0 // we need 8 pulses per round and we have a gear ratio of 120, so roud of wheel = 8*120=960
#define GET_PULSES_WHEEL(degrees) (float)(degrees*(ROUND_WHEEL360/360.0))

#define DIAMETER_WHEEL        6.5 //In centimeters
#define NUMBER_PI             3.141592654
#define DEGREES_ROTATE_1CM    1.0*(360.0/(DIAMETER_WHEEL*NUMBER_PI))
#define PULSES_1CM            (ROUND_WHEEL360/360.0)*DEGREES_ROTATE_1CM
#define GET_PULSES_MOVE(centimeters)  (uint16_t)(centimeters*PULSES_1CM)




/*
 *
 *************  INITIALIZATION OF MODULE *************
 *
 */

void initMotor(int ENA, int ENB, int IN1, int IN2, int IN3, int IN4);
void initEnconder(int encodRightPin, int encodLeftPin);



/*
 *
 *************  RUN MOTOR AND SET DIRECTION OF ROTATION *************
 *
 */

void setDirectionMotor(char motors, char directionMotor);
void analogWriteMotor(char motors,int pwm);
void runMotor(char motors,uint8_t rpmWheel); // may be, instead of float we will use int.



/*
 *
 *************  ASSING PWM TO RPM *************
 *
 */

void getRPM_BMotor(int pwm,float *rpmWheelR, float *rpmWheelL, unsigned long timeSample);
void getRPM_Motor(int pwm,float *rpmWheel, char motor, unsigned long timeSample);



/*
 *
 *************  ROTATE MOTOR *************
 *
 */

void powerRotationMotor(uint8_t power);
void rotateWheelMotor(char motors, uint16_t degrees);
void rotateCarMotor(char motors, uint16_t degrees);
void moveCentimetersMotor(char motors, uint8_t centimeters);



/*
 *
 *************  PID MOTOR *************
 *
 */

 void initPIDMotor(char motor,uint8_t setPoint, float Kp, float Ki, float Kd, unsigned long timeSample);
 void endPIDMotor(void);
 void executePIDMotor(int *valueRPM);
 void execute2PIDMotor(int *rightRPM, int *leftRPM);



#endif
