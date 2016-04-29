//**************************************
//*霍爾感測器
//*2016-04-29
//*吳昱成
//**************************************
#ifndef Hall_h
#define Hall_h

#include "Arduino.h"

class Hall {
public:
	Hall(int, double, int, double);
    void set(int, double, int, double);
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
    double _r;
    double _magnetR;
    double _pTheta; 	
};

#endif
