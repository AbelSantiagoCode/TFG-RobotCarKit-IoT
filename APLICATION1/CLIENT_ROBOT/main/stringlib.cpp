#include "stringlib.h"

bool equalStrings(char str1[], char str2[], uint8_t limit){
  for (uint8_t i = 0; i < limit; i++) {
    if (str1[i] != str2[i]) return false;
  }
  return true;
}

bool beginWithString(char str1[], char str2[]){
  for (uint8_t i = 0; str2[i] != '\0'; i++) {
    if (str1[i] != str2[i]) return false;
  }
  return true;
}

void appendString(char buffer[], uint8_t lenBuffer, char c){
  int i=0;
  while (buffer[i] != '\0') {
    i++;
  }
  if (i < lenBuffer-1) {
    buffer[i] = c;
    buffer[i+1] = '\0';
  }
}

void appendArrayString(char buffer[], uint8_t lenBuffer, char buffer2[]){
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

void shiftLeftString(char str[], unsigned int shift) {
  for (unsigned int j = 0; j < shift; j++) {
    for (unsigned int i = 0; str[i] != '\0'; i++) {
      str[i]= str[i+1];
    }
  }
}

void copyString(char str1[], char str2[]) {
  unsigned int i = 0;
  for (;  str2[i] != '\0' ; i++) {
    str1[i] = str2[i];
  }
  str1[i] = '\0';
}

unsigned int lenString(char str[]) {
  unsigned int len = 0;
  while (str[len]!='\0') {
    len++;
  }
  return len;
}
