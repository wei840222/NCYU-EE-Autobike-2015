//**************************************
//*霍爾感測器類別
//*2016-03-04
//*吳昱成
//**************************************
#include"Hall.h"
class Hall{

  //建構元
  void Hall() {
  } 
  void Hall(double lenth) {
    pml = lenth;
  }
  //實體方法
  //更新
  void stateUpdate(){     //請把我設定成外部中斷, FALLING mode       
    this.nowTime = millis();
    if(this.preTime==0) {
        this.preTime = this.nowTime;
    }else if(this.preSpeed==0){
        this.nowSpeed = this.pml/(this.nowTime-this.preTime)*36;    //*36 得(km/hr)
        this.preTime = this.nowTime;
        this.preSpeed = this.nowSpeed;
    }else {
        this.nowSpeed = this.pml/(this.nowTime-this.preTime)*36;
        this.nowAcc = (this.nowSpeed-this.preSpeed)/(this.nowTime-this.preTime);
        this.preTime = this.nowTime;
        this.preSpeed = this.nowSpeed;
    }
  }
  //傳值
  double getSpeed() {
    //stateUpdate();    
    return this.nowSpeed;
  }
  double getAcc() {
    //stateUpdate();
    return this.nowAcc;
  }
}
