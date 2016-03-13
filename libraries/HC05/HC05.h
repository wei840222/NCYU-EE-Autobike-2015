#ifndef HC05_h
#define HC05_h

#include "Arduino.h"

class HC05 {
public:
	String read();
	void write(String input);
private:
};

#endif