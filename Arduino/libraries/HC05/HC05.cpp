#include "HC05.h"

HC05::HC05(int baudrate) {
	Serial.begin(baudrate);
}

String HC05::read() {
  byte buffer;
  String input;
  if (Serial.available()) {
    while (Serial.available()) {
      buffer = Serial.read();
	  input += (char)buffer;
    }
  }
  return input;
}

void HC05::write(String output) {
  char buffer[32];
  output.toCharArray(buffer, output.length());
  if (Serial.available())
    for (int i = 0;i < output.length();i++)
      Serial.write((byte)buffer[i]);
}