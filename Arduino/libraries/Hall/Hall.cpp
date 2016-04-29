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
	_pml = 2 * PI*_magnetR/_magnetN;
	_preTime = 0;       //前一個時間點
	_nowTime = 0;       //現在的時間點
	_preSpeed = 0;
	_nowSpeed = 0;
	_nowAcc = 0;
	pinMode(_pin, INPUT_PULLUP);
}
void Hall::set(int pin, double r, int magnetN, double magnetR) {
  _pin = pin; 
  _r = r; 
  _magnetN = magnetN;
  _magnetR = magnetR;
	_pml = 2 * PI*_magnetR/_magnetN;
  _preTime = 0;       //前一個時間點
  _nowTime = 0;       //現在的時間點
  _preSpeed = 0;
  _nowSpeed = 0;
  _nowAcc = 0;    
  pinMode(_pin, INPUT_PULLUP);
}
void Hall::stateUpdate(){
  _nowTime = millis();
  if(_preTime==0) {
    _preTime = _nowTime;
  }else if(_preSpeed==0){
    _nowSpeed = _pml/(_nowTime-_preTime);   
    _preTime = _nowTime;
    _preSpeed = _nowSpeed;
  }else {
    _nowSpeed = _pml/(_nowTime-_preTime);
    _nowAcc = (_nowSpeed-_preSpeed)/(_nowTime-_preTime);  
    _preTime = _nowTime;
    _preSpeed = _nowSpeed;
  }
}
double Hall::getOmega() {
  return (_nowSpeed/1000)/_magnetR; // (rad/s)
}
double Hall::getAlpha() {
  return _nowAcc;  //" *10.0 "得(rad/s^2) 
}
