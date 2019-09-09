#ifndef TCPCLIENT
#define TCPCLIENT

#include <stdint.h>
//CONNECTION WITH ACCES POINT
void connectAccesPoint(char ssid[], char password[]);
void closeConnecAccesPoint(void);

//ESTABLISH COMMUNICATION WITH SERVER
bool connectTCPClient(byte ip[],uint16_t port);
void closeConnectTCPClient(void);
bool connectedTCPClient(void);

//FOR SOCKET APPLICATION
int availableServerMessage(void);
int readServerMessage(char buffer[], int limitBuffer);
int readServerLine(char buffer[], int limitBuffer);
int sendServerMessage(char buffer[]);


//FOR HTTP APPLICATION
void requestServerHTTP(char type, char path[], char parameters[]);
void configrequestServerHTTP(char startLine[], char headers[], char body[]);
void responseServerHTTP(char response[], int lenResponse);

#endif
