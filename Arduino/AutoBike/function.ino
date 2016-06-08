//*********************************************************
// 裝置測試
//*********************************************************
void testLCD() {
  LCD1602.clear();
  LCD1602.print("LED is OK!");
  delay(1500);
  LCD1602.clear();
}
void testSerial() {
  while(!Serial.available()) {
    LCD1602.clear();
    LCD1602.print("BT connect failed");
    delay(1000);
    LCD1602.clear();
  }
}
void testGY521() {
  while(!GY521.testConnection()) {
    LCD1602.clear();
    LCD1602.print("GY521 connect failed!");
    delay(1000);
    LCD1602.clear();
  }
}
//*********************************************************
// 更新函式
//*********************************************************
void drivesUpdate() {
  // update 單車的角度
  pre_gySlope = gySlope;
  gySlope = -getAngleY();
  
  // update 單車的速度
  double get_bikeSpeed = Wheel.getOmega()*wheel_R*3.6; // (km/hr)
  bikeSpeed = get_bikeSpeed;
  pre_bikeSpeed = bikeSpeed;
  
  // update 單車的rps & rpm
  double get_rps = Gear.getOmega()/2/PI;
  rps = get_rps;
  pre_rps = rps;
  rpm = rps * 60;
  
  // update 單車的加速度
  acceleration = Wheel.getAlpha()*wheel_R;
  
  // update 腳踏力道 & 腳踏功率
  pre_pedalTorque = pedalTorque;
  pedalTorque = getPedalTorque();
  pre_pedalPower = pedalPower;
  pedalPower = getPedalPower();
}

void showLCD() {
  // line 0
  LCD1602.clear();
  LCD1602.print("P:"); LCD1602.print((int)(255*PWM));
  LCD1602.print(" S:"); LCD1602.print((int)gySlope);
  // line 1
  LCD1602.setCursor(0, 1);
  LCD1602.print("V:"); LCD1602.print((int)bikeSpeed);
  LCD1602.print(" "); LCD1602.print((int)pedalTorque);
  // end
  delay(100);
}
void syncBT() {
  String input;
  String output;
  // recive
  /*if(Serial.available() > 0) {
    input = BT.read();
    // 判斷指令
    if(input=="auto")
      autoMode = true;
    if(input=="off")
      autoMode = false;
  }*/
  // send
  output = "Slope:" + (String)gySlope + " Speed:" + (String)bikeSpeed;
  BT.write(output);
  output = "";
  delay(100);
}

//*********************************************************
// 計算Y軸角度值
//*********************************************************
//單位SI制
double getAngleY() {
  int16_t ax, ay, az;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan(ay / sqrt(pow(ax, 2) + pow(az, 2)));
}

//*********************************************************
// 計算腳踏力量
//*********************************************************
double getPedalTorque() {
  if(abs(I*Gear.getAlpha())>0)
    return I*Gear.getAlpha();
    // (N-m)
  else
    return 0;
}
double getPedalPower() {
  if(abs(I*Gear.getAlpha())>0)
    return I*Gear.getAlpha()*Gear.getOmega();// (N-m)/s) = (W);
  else
    return 0;
}

//*********************************************************
// PWM 輸出
//*********************************************************
double PWMValue(double Spd, double deg) {
  double out = 0, P_reward = 0.0;
  abs(pedalPower-pre_pedalPower)<0.1?P_reward = 0.0:(pedalPower-pre_pedalPower)>0?P_reward = -0.05:P_reward = 0.05;
  if(Spd>=0 && Spd<15) {
    if(deg>=0 && deg<45 ) {
      double Slope = tan(deg*DEG_TO_RAD);
      out = 100*Slope+100*(1-Slope)/15*Spd;
    }else if(deg<10){
      out = 0;
    }else if(deg>45){
      out = 100;
    }
  }else if(Spd>=15 && Spd<25) {
    out = 100*exp((Spd-15)/(Spd-25));
  }else if(Spd>=25){
    out = 0;
  }
  return (1+P_reward)*out/100;
}
double reward(double Spd) {
  if(abs(Spd - bestBikeSpeed)<bestBikeSpeed_interval) {
    return bestBikeSpeed_interval/abs(Spd - bestBikeSpeed)/100;
  }else {
    return -abs(Spd - bestBikeSpeed)/100;
  }
}
void PWMOutput() {
  double prePWM;
  if(bikeSpeed<25){
    analogWrite(pin_pwm_output, (int)(255*PWM));
    analogWrite(11, (int)(255*PWM));
  }
  else {
    analogWrite(pin_pwm_output, 0);
    analogWrite(11, 0);
  }
  PWM = exp(-N)*(PWMValue(bikeSpeed, gySlope));
  prePWM = PWM;
  if(PWM<0) PWM = 0.0;
  if(PWM>1) PWM = 1.0;
}
