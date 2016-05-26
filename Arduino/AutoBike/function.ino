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

  // update 單車的速度 & rps & rpm
  pre_bikeSpeed = bikeSpeed;
  bikeSpeed = Wheel.getOmega()*wheel_R*3.6; // (km/hr)
  pre_rps = rps;
  rps = Gear.getOmega()/2/PI;
  rpm = rps * 60;

  // update 單車的加速度
  acceleration = Wheel.getAlpha()*wheel_R;

  // update 腳踏力道 & 腳踏功率
  pre_pedalTorque = pedalTorque;
  pedalTorque = getPedalTorque();
  pedalPower = getPedalPower();
}

void showLCD() {
  // line 0
  LCD1602.clear();
  LCD1602.print("");
  if(bikeSpeed>0 && bikeSpeed<15){
      LCD1602.print((int)abs(pedalPower/pedalPower_MAX*255+ abs(gySlope)));
  }else if(bikeSpeed>15){
      LCD1602.print((int)abs(pedalPower/pedalPower_MAX*(1-(int)(bikeSpeed-15)/9)*255+ abs(gySlope)));
  }
  LCD1602.print("  ");
  LCD1602.print(gySlope);

  // line 1
  LCD1602.setCursor(0, 1);
  LCD1602.print("V: ");
  LCD1602.print(bikeSpeed);
  LCD1602.print("  ");
  LCD1602.print(rpm);

  // end
  delay(1000);
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
  output += (String)gySlope + ":";
  output += (String)bikeSpeed;
  BT.write(output);
  output = "";
  delay(100);
}

//*********************************************************
// 計算Y軸角度值
//*********************************************************
//單位SI制
double getAngleX() {
  int16_t ax, ay, az;
  double Vax_offset = acceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = acceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ax - Vax_offset) / sqrt(pow(ay - Vay_offset, 2) + pow(az, 2)));
}

double getAngleY() {
  int16_t ax, ay, az;
  double Vax_offset = acceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = acceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}

double getAngleZ() {
  int16_t ax, ay, az;
  double Vax_offset = acceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = acceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((az) / sqrt(pow(ax - Vax_offset, 2) + pow(ay - Vay_offset, 2)));
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
  double out = 0;
  if(Spd>=0 && Spd<15) {
    if(deg>=0 && deg<90) {
      double Slope = tan(deg*DEG_TO_RAD);
      out = 100*Slope+100*(0.5-Slope)/15*Spd;
    }
  }else if(Spd>=15 && Spd<25) {
    out = 100*exp((Spd-15)/(Spd-25));
  }else {
    out = 0;
  }
  return out;
}
double reward(double Spd) {
  if(abs(Spd - bestBikeSpeed)<bestBikeSpeed_interval) {
    return bestBikeSpeed_interval/abs(Spd - bestBikeSpeed);
  }else {
    return -abs(Spd - bestBikeSpeed);
  }
}
void PWMOutput() {
  analogWrite(pin_pwm_output, 255*PWM);
  PWM = (int)((1-exp(-i))*PWM+exp(-i)(reward(bikeSpeed)+0.05*PWMValue(bikeSpeed, gySlope)));
  i++;
}
