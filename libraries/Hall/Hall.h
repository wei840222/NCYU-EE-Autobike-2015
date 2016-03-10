//**************************************
//*霍爾感測器
//*2016-03-10
//*吳昱成
//**************************************
#ifndef Hall_h
#define Hall_h

#include "Arduino.h"

class Hall
{
  public:
    void set(int pin, float r);
    void stateUpdate();
    float getSpeed();
    float getAcc();
  private:
    int _pin;
    float _r;
    float _pml;
    float _preTime;       //前一個時間點
    float _nowTime;       //現在的時間點
    float _preSpeed;
    float _nowSpeed;
    float _nowAcc;    
};

#endif
