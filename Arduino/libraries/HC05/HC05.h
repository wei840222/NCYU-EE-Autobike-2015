#ifndef _HC05_H_
#define _HC05_H_

#include "Arduino.h"

class HC05 {
public:
	HC05(int);
	String read();
	void write(String);
};

#endif