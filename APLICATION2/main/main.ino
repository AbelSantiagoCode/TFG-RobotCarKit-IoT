#include "tcpserver.h"
#include "stringlib.h"
#include "program1.h"
// #include <SPI.h>
// #include <WiFiNINA.h>
#define LMESSAGE    50

#define SECRET_SSID "vodafone****"
#define  SECRET_PASS "*********";
#define LEN_PATH   100
#define LEN_QUEUE 200
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// GLOBAL VARIABLES
typedef enum state_t {S1, S2};


//  PROTOTYPES
bool getEventFSM(char message[]);
void FSM_Main(void);


//  CODE
bool getEventFSM(char message[]){
  char listPATH[LEN_PATH];
  char listQUEUE[LEN_QUEUE];
  char tipusRequest;
  if (clientAvailable()) {
    // result = 0 neither parameters nor body, result = 1 only parameters, result = 2 only body ,result = 3 both parameters and body
    uint8_t tpHTTP = readRequestHTTP(listPATH,LEN_PATH,&tipusRequest);
    if ((tipusRequest == 'G') && (tpHTTP == 1) && (beginWithString(listPATH,"/api/sendMessage") == true)) { // http://host:port/api/sendMessage?message=COMMAND
        //DEBUG
        Serial.println();
        Serial.println("GET HTTP REQUEST WITH PARAMETERS");
        Serial.println(listPATH);
        getParametersRequestHTTP(listQUEUE, LEN_QUEUE);
        //DEBUG
        Serial.println(listQUEUE);
        if (beginWithString(listQUEUE,"message=")) {
          shiftLeftString(listQUEUE,lenString("message="));
          Serial.println(lenString("message="));
          Serial.println(listQUEUE);
          copyString(message,listQUEUE);
          sendResponseHTTP(true,"ACK_OK");
          clientClose();
          return true;
        }
        sendResponseHTTP(true,"ACK_ERROR");
        clientClose();
        return false;
    }
    sendResponseHTTP(true,"ACK_ERROR");
    clientClose();
  }
  return false;
}


void FSM_Main(void){
  static state_t state = S1;
  static bool FlagPSelected = false;
  static char message[LMESSAGE+1];
  static bool event = false;

  switch (state) {
    case S1:
      event= getEventFSM(message);
      if ((event==true) && (beginWithString(message,"INIT")==true) ) {
        //DEBUG
        Serial.println("STATE 1 -> STATE2");
        initProgram1();
        FlagPSelected = true;
        state = S2;
      }

      break;
    case S2:
      if (FlagPSelected == true) {
        event = getEventFSM(message);
        FSM_PRG1(event, message, &FlagPSelected);
      } else {
        //DEBUG
        Serial.println("STATE 2 -> STATE1");
        state = S1;
      }
      break;
  }
}


void setup() {
  Serial.begin(9600);      // initialize serial communication
  connectAccesPoint(ssid,pass);
  initServer();                             // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status

}

void loop() {
  FSM_Main();
}



/******************************OBSERVAR SI FUNCIONA****************************/
void printWifiStatus() {
  // print where to go in a browser:
  int ipServer[4];
  ipAddressServer(ipServer);
  Serial.print("To see this page in action, open a browser to http://");
  Serial.print(ipServer[0]);
  Serial.print(".");
  Serial.print(ipServer[1]);
  Serial.print(".");
  Serial.print(ipServer[2]);
  Serial.print(".");
  Serial.println(ipServer[3]);

}
