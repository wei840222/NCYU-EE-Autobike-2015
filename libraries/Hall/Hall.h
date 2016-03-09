//**************************************
//*霍爾感測器類別
//*2016-03-04
//*吳昱成
//**************************************

class Hall{
	//運算用常數
	const float r = 29;				//(cm)
  //@變數
  double preTime = 0;       //前一個時間點
  double nowTime = 0;       //現在的時間點
  double preSpeed = 0;
  double nowSpeed = 0;
  double nowAcc = 0;
  double pml = 2*pi*r;
  //建構元
  void Hall();
  void Hall(double lenth);
  //更新
  void stateUpdate();
  //傳值
  double getSpeed();
  double getAcc();
}
