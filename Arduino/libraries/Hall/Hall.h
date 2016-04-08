//**************************************
//*霍爾感測器
//*2016-03-10
//*吳昱成
//**************************************
#ifndef Hall_h
#define Hall_h

#include "Arduino.h"

class Hall {
public:
  Hall(int, double);
    void set(int, double);
    void stateUpdate();
    double getSpeed();
    double getAcc();
private:
    int _pin;
    double _r;
    double _pml;
    volatile double _preTime;       //前一個時間點
    volatile double _nowTime;       //現在的時間點
    volatile double _preSpeed;
    volatile double _nowSpeed;
    volatile double _nowAcc;
};

#endif
