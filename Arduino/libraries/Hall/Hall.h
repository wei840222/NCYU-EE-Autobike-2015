//**************************************
//*霍爾感測器
//*2016-05-05
//*吳昱成
//**************************************
#ifndef Hall_h
#define Hall_h

#include "Arduino.h"

class Hall {
public:
	Hall(int, int);
    void set(int, int);
    void stateUpdate();
    double getOmega();
    double getAlpha();
    volatile double _preTime;       //前一個時間點
    volatile double _nowTime;       //現在的時間點
    volatile double _preOmega;
    volatile double _nowOmega;
    volatile double _nowAlpha;  
private:
    int _pin;
    int _magnetN;
    double _pTheta; 	
};

#endif
