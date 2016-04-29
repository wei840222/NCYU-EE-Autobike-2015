//**************************************
//*霍爾感測器
//*2016-04-29
//*吳昱成
//**************************************
#include "Hall.h"

Hall::Hall(int pin, double r, int magnetN, double magnetR) {
	_pin = pin;
	_r = r;
  _magnetN = magnetN;
  _magnetR = magnetR;
	_pTheta = 2 * PI/_magnetN;
	_preTime = 0;       //前一個時間點
	_nowTime = 0;       //現在的時間點
	_preOmega = 0;
	_nowOmega = 0;
	_nowAlpha = 0;
	pinMode(_pin, INPUT_PULLUP);
}
void Hall::set(int pin, double r, int magnetN, double magnetR) {
  _pin = pin; 
  _r = r; 
  _magnetN = magnetN;
  _magnetR = magnetR;
	_pTheta = 2 * PI/_magnetN;
  _preTime = 0;       //前一個時間點
  _nowTime = 0;       //現在的時間點
  _preOmega = 0;
  _nowOmega = 0;
  _nowAlpha = 0;    
  pinMode(_pin, INPUT_PULLUP);
}
void Hall::stateUpdate(){
  _nowTime = millis();
  if(_preTime==0) {
    _preTime = _nowTime;
  }else if(_preOmega==0){
    _nowOmega = _pTheta/(_nowTime-_preTime);   
    _preTime = _nowTime;
    _preOmega = _nowOmega;
  }else {
    _nowOmega = _pTheta/(_nowTime-_preTime);
    _nowAlpha = (_nowOmega-_preOmega)/(_nowTime-_preTime);  
    _preTime = _nowTime;
    _preOmega = _nowOmega;
  }
}
double Hall::getOmega() {
  return _nowOmega*1000; // (rad/s)
}
double Hall::getAlpha() {
  return _nowAlpha*1000*1000;  //(rad/s^2) 
}
