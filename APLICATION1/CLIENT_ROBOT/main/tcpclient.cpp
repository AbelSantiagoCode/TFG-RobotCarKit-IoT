#include <SPI.h>
#include <WiFiNINA.h>
#include <stdio.h>
#include "tcpclient.h"

#define URL_LEN 100
#define HOST_LEN 50
#define BUFFER_LEN 100

int status  = WL_IDLE_STATUS; // the wifi radio's status.
WiFiClient clientTCPIP;
static byte serverTCPIP[4];
static uint16_t portTCPIP;


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





/***************************ESTABLISH COMMUNICATION WITH SERVER***************************/
bool connectTCPClient(byte ip[],uint16_t port){
  if (!(clientTCPIP.connect(ip,port))){
    //CONNECTION TO SERVER FAILS
    return false;
  }
  serverTCPIP[0] = ip[0];
  serverTCPIP[1] = ip[1];
  serverTCPIP[2] = ip[2];
  serverTCPIP[3] = ip[3];
  portTCPIP = port;
  return true;
}

void closeConnectTCPClient(void){
  clientTCPIP.stop();
}

bool connectedTCPClient(void){
  // if the server's disconnected, stop the client:
  clientTCPIP.connected();
}
/***************************ESTABLISH COMMUNICATION WITH SERVER***************************/





/***************************FOR SOCKET APPLICATION***************************/
int availableServerMessage(void){
  return clientTCPIP.available();
}

int readServerMessage(char buffer[], int limitBuffer){
  int available = clientTCPIP.available();
  //If there are not available bytes to read.
  if (available == 0) {
    return 0;
  }
  int limit = 0;
  //If there are incoming bytes availables read  them.
  if (available <= limitBuffer) {
    limit = available;
  } else {
    limit = limitBuffer;
  }

  for (int i = 0 ;i < limit ;i++){
    buffer[i] = clientTCPIP.read();
  }
  buffer[limit] = '\0';
  return limit;
}

int readServerLine(char buffer[], int limitBuffer){
  int limit = 0;
  int available = clientTCPIP.available();
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
    if (clientTCPIP.connected()){
      buffer[index] = clientTCPIP.read();
      if (buffer[index] == '\n') {
        break;
      }
    }
  }
  buffer[index+1] = '\0';
  return index+1;
}

int  sendServerMessage(char buffer[]){
  return clientTCPIP.println(buffer);
}
/***************************FOR SOCKET APPLICATION***************************/





/***************************FOR HTTP APPLICATION***************************/
void requestServerHTTP(char type, char path[], char message[]){
  char urlRequest[URL_LEN] = "";
  char host[HOST_LEN] = "Host: ";
  char port[10];
  char ip0[4]; char ip1[4]; char ip2[4]; char ip3[4];

  sprintf(port,"%d",portTCPIP);
  sprintf(ip0,"%d",serverTCPIP[0]);
  sprintf(ip1,"%d",serverTCPIP[1]);
  sprintf(ip2,"%d",serverTCPIP[2]);
  sprintf(ip3,"%d",serverTCPIP[3]);


  appendArrayString(host, HOST_LEN, ip0);
  appendString(host, HOST_LEN, '.');
  appendArrayString(host, HOST_LEN, ip1);
  appendString(host, HOST_LEN, '.');
  appendArrayString(host, HOST_LEN, ip2);
  appendString(host, HOST_LEN, '.');
  appendArrayString(host, HOST_LEN, ip3);
  appendString(host, HOST_LEN, ':');
  appendArrayString(host, HOST_LEN, port);


  // close any connection before send a new request.
  // This will free the socket on the Nina module
  clientTCPIP.stop();
  // if there's a successful connection:
  if(clientTCPIP.connect(serverTCPIP,portTCPIP)){
    if (type == 'G') { //type http request GET
      //send the HTTP PUT request:
      appendArrayString(urlRequest, URL_LEN, "GET " );
      appendArrayString(urlRequest, URL_LEN, path);
      if (message[0] == '\0') {
        appendArrayString(urlRequest, URL_LEN, " HTTP/1.1");
      } else {
        appendString(urlRequest, URL_LEN, '?');
        appendArrayString(urlRequest, URL_LEN, message);
        appendArrayString(urlRequest, URL_LEN, " HTTP/1.1");
      }
      clientTCPIP.println(urlRequest); //GET /api/test HTTP/1.1
      clientTCPIP.println(host); //Host: 192.168.0.161:8000
      clientTCPIP.println("User-Agent: ArduinoWiFi/1.1");
      clientTCPIP.println("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
      clientTCPIP.println("Connection: keep-alive");
      clientTCPIP.println();
    }
    if (type == 'P') { //type http request POST oriented to forms
      //send the HTTP PUT request:
      int content_len = 0;
      char contetLen[10];
      char content[50] = "Content-Length: ";
      sprintf(contetLen,"%d",lenString(message));
      appendArrayString(content, 50, contetLen);
      appendArrayString(urlRequest, URL_LEN, "POST " );
      appendArrayString(urlRequest, URL_LEN, path);
      appendArrayString(urlRequest, URL_LEN, " HTTP/1.1");
      clientTCPIP.println(urlRequest); //POST /api/test HTTP/1.1
      clientTCPIP.println(host); //Host: 192.168.0.161:8000
      clientTCPIP.println("User-Agent: ArduinoWiFi/1.1");
      clientTCPIP.println("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
      clientTCPIP.println("Content-Type: application/x-www-form-urlencoded");
      clientTCPIP.println("Connection: keep-alive");
      clientTCPIP.println(content); //Content-Length: 22
      clientTCPIP.println();
      clientTCPIP.print(message);//fname=abel1&lname=noel
      clientTCPIP.println();
    }
  }
}

void configrequestServerHTTP(char startLine[], char headers[], char body[]){

  char host[HOST_LEN] = "Host: ";
  char port[10];
  char ip0[4]; char ip1[4]; char ip2[4]; char ip3[4];

  sprintf(port,"%d",portTCPIP);
  sprintf(ip0,"%d",serverTCPIP[0]);
  sprintf(ip1,"%d",serverTCPIP[1]);
  sprintf(ip2,"%d",serverTCPIP[2]);
  sprintf(ip3,"%d",serverTCPIP[3]);


  appendArrayString(host, HOST_LEN, ip0);
  appendString(host, HOST_LEN, '.');
  appendArrayString(host, HOST_LEN, ip1);
  appendString(host, HOST_LEN, '.');
  appendArrayString(host, HOST_LEN, ip2);
  appendString(host, HOST_LEN, '.');
  appendArrayString(host, HOST_LEN, ip3);
  appendString(host, HOST_LEN, ':');
  appendArrayString(host, HOST_LEN, port);


  // close any connection before send a new request.
  // This will free the socket on the Nina module
  clientTCPIP.stop();
  // if there's a successful connection:
  if(clientTCPIP.connect(serverTCPIP,portTCPIP)){
    clientTCPIP.println(startLine); //GET /api/test HTTP/1.1
    clientTCPIP.println(host); //Host: 192.168.0.161:8000
    clientTCPIP.print(headers);
    clientTCPIP.println();
    if (body[0]!='\0') {
      clientTCPIP.print(body);
    }
  }

}

void responseServerHTTP(char response[], int lenResponse){
  char buffer[BUFFER_LEN];
  while (readServerLine(buffer, BUFFER_LEN) != 0) {
    if((buffer[0] == '\r') && (buffer[1] == '\n')){
      readServerMessage(response, lenResponse);
      break;
    }
  }
}
/***************************FOR HTTP APPLICATION***************************/
