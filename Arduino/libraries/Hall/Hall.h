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
	Hall(int, double, int);
    void set(int, double, int);
    void stateUpdate();
    double getSpeed();
    double getAcc();
    volatile double _preTime;       //前一個時間點
    volatile double _nowTime;       //現在的時間點
    volatile double _preSpeed;
    volatile double _nowSpeed;
    volatile double _nowAcc;  
private:
    int _pin;
    int _magnetN;
    double _r;
    double _pml; 	
};

#endif
