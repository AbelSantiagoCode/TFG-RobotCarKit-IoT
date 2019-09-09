#ifndef TCPSERVER
#define TCPSERVER
#define PORT_SERVER 80

#include <stdint.h>
//CONNECTION WITH ACCES POINT
void connectAccesPoint(char ssid[], char password[]);
void closeConnecAccesPoint(void);

//ESTABLISH-INITIALIZE COMMUNICATION WITH SERVER
void initServer(void);
void ipAddressServer(int *ipServer);
int portServer(void);
bool clientAvailable(void);
void clientClose(void);

//FOR SOCKET APPLICATION
int readClientMessage(char buffer[], int limitBuffer);
int readClientLine(char buffer[], int limitBuffer);
int sendClientMessage(char buffer[]);

//FOR HTTP APPLICATION
uint8_t readRequestHTTP(char path[], int limitPath, char *typeRequest); // result = 0 neither parameters nor body, result = 1 only parameters, result = 2 only body ,result = 3 both parameters and body
void getBodyRequestHTTP(char buffer[], int limitBuffer);
void getParametersRequestHTTP(char buffer[], int limitBuffer);
void sendResponseHTTP(bool statusCode, char body[]); //oriented to use with a json type data.
void configResponseHTTP(char lineStatus[], char headers[], char body[]);

#endif
