#include "tcpserver.h"
#include <stdio.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <stdint.h>

#define LEN_BUFFER 200

int status  = WL_IDLE_STATUS; // the wifi radio's status.
WiFiServer server(PORT_SERVER);
WiFiClient client;

static char bufferBodyHTTP[LEN_BUFFER];
static char bufferParametersHTTP[LEN_BUFFER];





/***************************TO WORK WITH STRINGS***************************/
static bool equalStrings(char str1[], char str2[], uint8_t limit);
static bool beginWithString(char str1[], char str2[]);
static void appendString(char buffer[], uint8_t lenBuffer, char c);
static void appendArrayString(char buffer[], uint8_t lenBuffer, char buffer2[]);
static void shiftLeftString(char str[], unsigned int shift);
static void copyString(char str1[], char str2[]);
static unsigned int lenString(char str[]);

static bool equalStrings(char str1[], char str2[], uint8_t limit){
  for (uint8_t i = 0; i < limit; i++) {
    if (str1[i] != str2[i]) return false;
  }
  return true;
}

static bool beginWithString(char str1[], char str2[]){
  for (uint8_t i = 0; str2[i] != '\0'; i++) {
    if (str1[i] != str2[i]) return false;
  }
  return true;
}

static void appendString(char buffer[], uint8_t lenBuffer, char c){
  int i=0;
  while (buffer[i] != '\0') {
    i++;
  }
  if (i < lenBuffer-1) {
    buffer[i] = c;
    buffer[i+1] = '\0';
  }
}

static void appendArrayString(char buffer[], uint8_t lenBuffer, char buffer2[]){
  int i = 0;
  int i2 = 0;
  while (buffer[i] != '\0') {
    i++;
  }
  while (buffer2[i2] != '\0') {
    i2++;
  }
  if (i+i2 < lenBuffer) {
    for (int j = 0; j < i2 ; j++) {
      buffer[i+j] = buffer2[j];
    }
    buffer[i+i2] = '\0';
  }
}

static void shiftLeftString(char str[], unsigned int shift) {
  for (unsigned int j = 0; j < shift; j++) {
    for (unsigned int i = 0; str[i] != '\0'; i++) {
      str[i]= str[i+1];
    }
  }
}

static void copyString(char str1[], char str2[]) {
  unsigned int i = 0;
  for (;  str2[i] != '\0' ; i++) {
    str1[i] = str2[i];
  }
  str1[i] = '\0';
}

static unsigned int lenString(char str[]) {
  unsigned int len = 0;
  while (str[len]!='\0') {
    len++;
  }
  return len;
}
/***************************TO WORK WITH STRINGS***************************/



/***************************CONNECTION WITH ACCES POINT***************************/

void connectAccesPoint(char ssid[], char password[]){
  // Check for the Wifi module:
  if (WiFi.status() == WL_NO_MODULE){
    // Don't continue.
    while(true);
  }
  // Attempt o connect wifi network.
  while (status != WL_CONNECTED){
    status = WiFi.begin(ssid,password);
    //Wait 10 seconds for connection  I MUST TO REPLACE THIS FUNCTION FOR ANOTHER
    delay(10000);
  }
}

void closeConnecAccesPoint(void){
  WiFi.end();
}
/***************************CONNECTION WITH ACCES POINT***************************/





/***************************ESTABLISH-INITIALIZE COMMUNICATION WITH SERVERT***************************/

void initServer(void) {
  server.begin();
}

void ipAddressServer(int *ipServer) {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  ipServer[0] = ip[0];
  ipServer[1] = ip[1];
  ipServer[2] = ip[2];
  ipServer[3] = ip[3];
}

int portServer(void) {
  return PORT_SERVER;
}

bool clientAvailable(void){
  client = server.available();   // listen for incoming clients
  if (client) {
    return true;
  }
  return false;
}

void clientClose(void){
  // close the connection:
    client.stop();
}
/***************************ESTABLISH-INITIALIZE COMMUNICATION WITH SERVERT***************************/





/***************************FOR SOCKET APPLICATION***************************/
int readClientLine(char buffer[], int limitBuffer){
  int limit = 0;
  int available = client.available();
  //If there are not available bytes to read.
  if (available == 0) {
    return limit;
  }
  //If there are incoming bytes availables read  them.
  if (available <= limitBuffer-1) {
    limit = available;
  } else {
    limit = limitBuffer-1;
  }
  int index = 0;
  for (index = 0 ;index < limit ;index++){
    if (client.connected()){
      buffer[index] = client.read();
      if (buffer[index] == '\n') {
        break;
      }
    }
  }
  buffer[index+1] = '\0';
  return index+1;
}

int readClientMessage(char buffer[], int limitBuffer){
  int limit = 0;
  int available = client.available();
  //If there are not available bytes to read.
  if (available == 0) {
    return limit;
  }
  //If there are incoming bytes availables read  them.
  if (available <= limitBuffer-1) {
    limit = available;
  } else {
    limit = limitBuffer-1;
  }
  for (int i = 0 ;i < limit ;i++){
    if (client.connected()){
      buffer[i] = client.read();
    }
  }
  buffer[limit] = '\0';
  return limit;
}

int sendClientMessage(char buffer[]){
  return server.print(buffer);
}
/***************************FOR SOCKET APPLICATION***************************/





/***************************FOR HTTP APPLICATION***************************/
uint8_t readRequestHTTP(char path[],int limitPath, char *typeRequest) { // result = 0 neither parameters nor body, result = 1 only parameters, result = 2 only body ,result = 3 both parameters and body// typeRequest = {'G'(get), 'P'(post)}
  int len = 0;
  char buffer[LEN_BUFFER]; // MAY BE WE NEED MORE INTERN BUFFER
  int indexBeginURL = 0;
  int indexEndURL  = -1;
  int indexBeginParameters = -1;
  int indexEndParameters = -1;
  int result = 0; // result = 0 neither parameters nor body, result = 1 only parameters, result = 2 only body ,result = 3 both parameters and body
  //POST /testPost HTTP/1.1
  //GET /testPost?fname=HOLA&lname=MUNDO HTTP/1.1
  while (true) {
    len = readClientLine(buffer,LEN_BUFFER);
    if (beginWithString(buffer,"GET /") || beginWithString(buffer,"POST /") ) {
      // CHECK IF IT'S A GET O POST REQUEST
      if (buffer[0] == 'G') { // GET HTTP
        indexBeginURL = 4;
        *typeRequest = 'G';
      } else {                // POST HTTP
        indexBeginURL = 5;
        *typeRequest = 'P';
      }
      //********* GET THE INDEX BEGIN OF PARAMETERS *********
      for (int i = indexBeginURL; i < len; i++) {
        if (buffer[i] == '?' ) { // /url....?var1=23&var2=23
          indexBeginParameters = i+1;
          break;
        }
      }
      //********* GET THE INDEX END OF URL *********
      for (int i = indexBeginURL; i < len; i++) {
        if ((buffer[i] == ' ') && (buffer[i+1] == 'H') && (buffer[i+2] == 'T') && (buffer[i+3] == 'T') && (buffer[i+4] == 'P') && (buffer[i+5] == '/') && (buffer[i+6] == '1') && (buffer[i+7] == '.') && (buffer[i+8] == '1') ) {
          indexEndURL = i-1;
          break;
        }
      }

      //********* THE URL COTAINS PARAMETERS *********
      if (indexBeginParameters != -1) {
        indexEndParameters = indexEndURL;
        indexEndURL = indexBeginParameters-2; // we need to take account the '?'
      }
      //********* LIMITATION OF ARRAY IF IT IS NECESSARY *********
      if (limitPath-2 < indexEndURL) { // we take account the '\0' at the end of the array
        indexEndURL = limitPath-2;
      }

      //********* SAVE THE URL-PATH *********
      int i2 = indexBeginURL;
      for ( int j = 0; i2 <= indexEndURL; i2++, j++) {
        path[j] = buffer[i2];
        if (i2 == indexEndURL) {
          path[j+1] = '\0';
        }
      }
      //********* SAVE PARAMETERS OF THE URL *********
      if (indexEndParameters != -1) {
        result = 1;
        int i3 = indexBeginParameters;
        for ( int j = 0; i3 <= indexEndParameters; i3++, j++) {
          bufferParametersHTTP[j] = buffer[i3];
          if (i3 == indexEndParameters) {
            bufferParametersHTTP[j+1] = '\0';
          }
        }
      }
      //********* RETURN FOR GET HTTP *********
      if ((*typeRequest) == 'G') {
        return result;
      }
    }

    //********* GET THE BODY MESSAGE *********
    if ((len == 2) && (buffer[0]=='\r') && (buffer[1]=='\n')) { // GET THE DATA ENCLOSED IN THE BODY OF THE REQUEST MESSAGE
      len = readClientMessage(bufferBodyHTTP,LEN_BUFFER);
      if (len > 0) { // THERE IS BODY MESSAGE IN THE HTTP REQUEST
        if (result == 1) {
          result = 3;
        } else {
          result = 2;
        }
      }
      return result;
    }
  }
}


void getBodyRequestHTTP(char buffer[], int limitBuffer) {
  int index = 0;
  for ( index = 0; (bufferBodyHTTP[index] != '\0') && (index < limitBuffer-1); index++) {
    buffer[index] = bufferBodyHTTP[index];
  }
  buffer[index] = '\0';
}

void getParametersRequestHTTP(char buffer[], int limitBuffer) {
  int index = 0;
  for ( index = 0; (bufferParametersHTTP[index] != '\0') && (index < limitBuffer-1); index++) {
    buffer[index] = bufferParametersHTTP[index];
  }
  buffer[index] = '\0';
}


void sendResponseHTTP(bool statusCode, char body[]){
  int lenBuffer;
  char strLenBuffer[10];
  char contentLength[50] = "Content-length:";
  lenBuffer = lenString(body);
  sprintf(strLenBuffer,"%d",lenBuffer);
  appendArrayString(contentLength, 50, strLenBuffer);

  if (statusCode == true) {
    client.println("HTTP/1.1 200 OK");
  } else {
    client.println("HTTP/1.1 404 Not Found");
  }
  client.println("Content-type:text/plain");
  client.println(contentLength);
  client.println("Access-Control-Allow-Origin: *");
  client.println();

  //client.print("{\"username\":\"EXAMPLEusername\"}");
  client.print(body);

}

void configResponseHTTP(char startLine[], char headers[], char body[]){
  client.println(startLine);
  client.print(headers);
  client.println();
  //client.print("{\"username\":\"EXAMPLEusername\"}");
  if (body[0]!='\0') {
    client.print(body);
  }

}
/***************************FOR HTTP APPLICATION***************************/
