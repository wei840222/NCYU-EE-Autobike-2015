#ifndef _HC05_H_
#define _HC05_H_

#include "Arduino.h"

class HC05 {
public:
	String read();
	void write(String input);
private:
};

#endif /* _HC05_H_ */