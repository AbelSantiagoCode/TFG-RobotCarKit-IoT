#include "tcpclient.h"
#include "stringlib.h"
#include "program2.h"
#include "program3.h"
#include "program1.h"
#include <stdio.h>
#define LMESSAGE    50


// GLOBAL VARIABLES
typedef enum state_t {S1, S2};
typedef enum programs_t {INIT, PRG1, PRG3, PRG4};

char ssidA[]="vodafone****";
char passwordA[]="*********";
byte serverA[4]={192,168,0,159};
uint16_t portA = 2000;

//  PROTOTYPES
bool getEventFSM(char message[]);
bool checkProgram(char message[], programs_t *program);
void tcpClientFSM(void);
void FSM_P3(bool event, char message[], bool *FlagPSelected);
void FSM_P4(bool event, char message[], bool *FlagPSelected);

//  CODE
bool getEventFSM(char message[]){
  if (readServerMessage(message, LMESSAGE) == 0) {
    return false;
  }
  return true;
}

bool checkProgram(char message[], programs_t *program) {
  //DEBUG
  Serial.println("**************** CHECKPROGRAM TEST ******************");
  for (uint8_t i = 0; message[i] != '\0' ; i++) {
    Serial.print(message[i]);
  }
  Serial.println(" ");
  if (beginWithString(message,"PRG1")) {
    *program = PRG1;
    initProgram1();
    return true;
  }
  else if (beginWithString(message,"PRG3")) {
    *program = PRG3;
    initProgram3();
    return true;
  }
  else if (beginWithString(message,"PRG4")) {
    *program = PRG4;
    initProgram4();
    return true;
  }
  else {return false;}
}

void FSM_P3(bool event, char message[], bool *FlagPSelected){
  static bool FlagSendWeb = false;
  int degree = 0;
  int distance = 0;
  char strDegree[10];
  char strDistance[10];
  char responsePRG3[50]="programa3,";

  //Send data only when PID is executting. It will be aviable when message START is sent.
  if (event == true) {
    if (beginWithString(message,"START")) {
      FlagSendWeb = true;
    }
    else if (beginWithString(message,"STOP") || beginWithString(message,"EXIT")) {
      FlagSendWeb = false;
    }
  }


  //Call to the FSM of program4
  FSM_PRG3(event, message, FlagPSelected, &degree, &distance);

  //Send data only when distance is higher than stablished threshold.
  if ((FlagSendWeb == true) && (distance>0)) {
    sprintf(strDegree,"%d",degree);
    sprintf(strDistance,"%d",distance);
    appendArrayString(responsePRG3,50,strDegree);
    appendString(responsePRG3,50,',');
    appendArrayString(responsePRG3,50,strDistance);
    sendServerMessage(responsePRG3);
  }
}


void FSM_P4(bool event, char message[], bool *FlagPSelected){
  static bool FlagSendWeb = false;
  int rightRPM = -1;
  int leftRPM = -1;
  char right_rpm[10];
  char left_rpm[10];
  char responsePRG4[50]="programa4,";

  //Send data only when PID is executting. It will be aviable when message START is sent.
  if (event == true) {
    if (beginWithString(message,"START")) {
      FlagSendWeb = true;
    }
    else if (beginWithString(message,"STOP") || beginWithString(message,"EXIT")) {
      FlagSendWeb = false;
    }
  }

  //Call to the FSM of program4
  FSM_PRG4(event, message, &rightRPM, &leftRPM, FlagPSelected);

  //Send data only when PID is executting.
  if ((FlagSendWeb == true) && (rightRPM!=-1) && (leftRPM!=-1)) {
    sprintf(right_rpm,"%d",rightRPM);
    sprintf(left_rpm,"%d",leftRPM);
    appendArrayString(responsePRG4,50,left_rpm);
    appendString(responsePRG4,50,',');
    appendArrayString(responsePRG4,50,right_rpm);
    sendServerMessage(responsePRG4);
  }
}



void tcpClientFSM(void){
  static state_t state = S1;
  static bool FlagPSelected = false;
  static char message[LMESSAGE+1];
  static bool event = false;
  static programs_t program = INIT;

  switch (state) {
    case S1:
      event= getEventFSM(message);
      if ((event==true) && (checkProgram(message,&program)==true) ) {
        //DEBUG
        Serial.println("STATE 1 -> STATE2");
        FlagPSelected = true;
        state = S2;
      }
      else if ((event==true) && (beginWithString(message,"EXIT")==true) ) {
        //DEBUG
        Serial.println("TCP CLIENT DISCONNECTED");
        closeConnectTCPClient();
      }
      break;
    case S2:
      if (FlagPSelected == true) {
        event = getEventFSM(message);
        switch (program) {
          case PRG1:
            FSM_PRG1(event, message, &FlagPSelected);
            break;
          case PRG3:
            FSM_P3(event, message, &FlagPSelected);
            break;
          case PRG4:
            FSM_P4(event, message, &FlagPSelected);
            break;
        }
      } else {
        //DEBUG
        Serial.println("STATE 2 -> STATE1");
        program = INIT;
        state = S1;
      }
      break;
  }
}


void setup() {
  //DEBUG
  Serial.begin(57600);//Initialize the serial port
  delay(10000);
  Serial.println("attempt wifi");

  connectAccesPoint(ssidA,passwordA);
  Serial.println("WIFI CONNECTED");
  connectTCPClient(serverA,portA);
  Serial.println("SOCKTED CONNECTED");
}

void loop() {
  tcpClientFSM();
}
