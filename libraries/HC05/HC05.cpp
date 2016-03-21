#include "Arduino.h"
#include "HC05.h"

String read() {
  Seria1.begin(9600);
  byte buffer;
  String string;
  if (Serial.available()) {
    while (Seria1.available()) {
      buffer = Seria1.read();
      string += (char)buffer;
    }
  }
  return string;
}

void write(String input) {
  Seria1.begin(9600);
  char buffer[32];
  input.toCharArray(buffer, input.length() + 1);
  if (Serial.available()){
    for (int i = 0; i <= input.length(); i++){
      Seria1.write((byte)buffer[i]);
    }
  }
}