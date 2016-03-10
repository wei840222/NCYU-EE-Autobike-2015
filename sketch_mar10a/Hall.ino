//**************************************
//*霍爾感測器類別
//*2016-03-04
//*吳昱成
//**************************************
class Hall{
  //************************************
  //變數區
  //************************************
  public:
    int _pin = 2;
    float _r = 29;
  private:				//(cm)
    float _pml = 2*PI*_r;
    float _preTime = 0;       //前一個時間點
    float _nowTime = 0;       //現在的時間點
    float _preSpeed = 0;
    float _nowSpeed = 0;
    float _nowAcc = 0;
  //************************************
  //方法區
  //************************************
  public:
    void set(int pin, float r) {
      _pin = pin; 
      _r = r;
      _pml = 2*PI*_r;
      pinMode(_pin, INPUT_PULLUP);
    }
    void stateUpdate(){
      _nowTime = millis();
      if(_preTime==0) {
        _preTime = _nowTime;
      }else if(_preSpeed==0){
        _nowSpeed = _pml/(_nowTime-_preTime)*36;    //" *36 " 得(km/hr)
        _preTime = _nowTime;
        _preSpeed = _nowSpeed;
      }else {
        _nowSpeed = _pml/(_nowTime-_preTime)*36;
        _nowAcc = (_nowSpeed-_preSpeed)/(_nowTime-_preTime)/3.6;//" /3.6 "得(m/s) 
        _preTime = _nowTime;
        _preSpeed = _nowSpeed;
      }
    }
    float getSpeed() {
      return _nowSpeed;
    }
    float getAcc() {
      return _nowAcc;
    }
};
