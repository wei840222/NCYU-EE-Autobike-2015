#include "HC05.h"

String HC05::read() {
	byte buffer;
	String string;
	if (Serial1.available()) {
		while (Serial1.available()) {
			buffer = Serial1.read();
			string += (char)buffer;
		}
	}
	return string;
}

void HC05::write(String input) {
	char buffer[32];
	input.toCharArray(buffer, input.length() + 1);
	if (Serial1.available())
		for (int i = 0; i <= input.length(); i++)
			Serial1.write((byte)buffer[i]);
}