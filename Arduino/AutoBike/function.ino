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
  gySlope = getAngleY();

  // update 單車的速度 & rps & rpm
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
  LCD1602.print(pedalPower, 8);

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
  if(Serial.available() > 0) {
    input = BT.read();
    // 判斷指令
    if(input=="$A") autoMode = !autoMode;
    // 顯示
    LCD1602.clear();
    LCD1602.print(input);
    delay(2000);
  }
  // send
  if(1/*input=="$INFOR"*/){
    output += (String)autoMode     + ";";
    output += (String)gySlope      + ";";
    output += (String)bikeSpeed    + ";";
    output += '\n';
    BT.write(output);
    // Serial.print(output);
    // Serial.println(bikeSpeed);
    // Serial.println(gySlope);
    Serial.println(pedalPower, 8);
  }
  //
  delay(100);
}

//*********************************************************
// 計算Y軸角度值
//*********************************************************
//單位SI制
double getAngleY() {
  int16_t ax, ay, az;
  double Vax_offset = acceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = acceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}

//*********************************************************
// 計算腳踏力量
//*********************************************************
double getPedalTorque() {
  if(abs(I*Gear.getAlpha())>0){
    return I*Gear.getAlpha(); // (N-m)
  }else {
    return 0;
  }
}

double getPedalPower() {
  if(abs(I*Gear.getAlpha())>0){
    return I*Gear.getAlpha()*Gear.getOmega();// (N-m)/s) = (W)
  }else {
    return 0;
  }
}

//*********************************************************
// PWM 輸出
//*********************************************************
void PWMOutput() {
  if(abs(gySlope) <= 5){
    if(bikeSpeed>0 && bikeSpeed<15) {
      analogWrite(pin_pwm_output, pedalPower/pedalPower_MAX*255);
    }else if(bikeSpeed>15 && bikeSpeed<24) {
      analogWrite(pin_pwm_output, pedalPower/pedalPower_MAX*(1-(int)(bikeSpeed-15)/9)*255);
    }
  }
}
