//**************************************
//*霍爾感測器
//*2016-03-10
//*吳昱成
//**************************************
#include "Hall.h"

Hall::Hall(int pin, double r) {
	_pin = pin;
	_r = r;
	_pml = 2 * PI*_r;
	_preTime = 0;       //前一個時間點
	_nowTime = 0;       //現在的時間點
	_preSpeed = 0;
	_nowSpeed = 0;
	_nowAcc = 0;
	pinMode(_pin, INPUT_PULLUP);
}
void Hall::set(int pin, double r) {
  _pin = pin; 
  _r = r;
  _pml = 2*PI*_r;
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
double Hall::getSpeed() {
  return _nowSpeed*36.0;  //" *36.0 " 得(km/hr)
}
double Hall::getAcc() {
  return _nowAcc*10.0;  //" *10.0 "得(m/s^2) 
}
