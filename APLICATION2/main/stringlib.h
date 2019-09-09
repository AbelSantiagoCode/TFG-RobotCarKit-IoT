#ifndef STRINGLIB
#define STRINGLIB


#include <stdint.h>
bool equalStrings(char str1[], char str2[], uint8_t limit);
bool beginWithString(char str1[], char str2[]);
void appendString(char buffer[], uint8_t lenBuffer, char c);
void appendArrayString(char buffer[], uint8_t lenBuffer, char buffer2[]);
void shiftLeftString(char str[], unsigned int shift);
void copyString(char str1[], char str2[]);
unsigned int lenString(char str[]);
#endif
