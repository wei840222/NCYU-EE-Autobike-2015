//*********************************************************
// 更新函式
//*********************************************************
void drivesUpdate() {
  //單車的角度
  pre_gySlope = gySlope;
  gySlope = -getAngleY();
  //單車的速度
  pre_bikeSpeed = bikeSpeed;
  bikeSpeed = Wheel.getOmega()*wheel_R*3.6; // (km/hr)
  //單車的加速度
  acceleration = Wheel.getAlpha()*wheel_R;
  //踏板rps & rpm
  pre_rps = rps;
  rps = Gear.getOmega()/2/PI;
  rpm = rps * 60;
  //腳踏力道 & 腳踏功率
  pre_pedalTorque = pedalTorque;
  pedalTorque = getPedalTorque();
  pre_pedalPower = pedalPower;
  pedalPower = getPedalPower();
}

void showLCD() {
  // line 0
  LCD1602.clear();
  LCD1602.print((int)(255*PWM));
  LCD1602.print(" ");
  LCD1602.print(gySlope);
  // line 1
  LCD1602.setCursor(0, 1);
  LCD1602.print("V:");
  LCD1602.print(bikeSpeed);
  LCD1602.print(" RPM:");
  LCD1602.print(rpm);
  // end
  delay(100);
}
void syncBT() {
  String input;
  String output;
  /*
  // recive
  if(Serial.available() > 0) {
    char buffer;
    while (Serial.available()) {
      buffer = Serial.read();
      input += (char)buffer;
    }
  }
  // 判斷指令
  if(input=="auto")
    autoMode = true;
  if(input=="off")
    autoMode = false;
  input = "";
  */
  // send
  output = "Slope: " + (String)gySlope + " Speed: " + (String)bikeSpeed + "km/h";
  char *buffer;
  output.toCharArray(buffer, output.length());
  if(Serial.available()) {
    for (int i = 0;i < output.length();i++) {
      Serial.write((byte)buffer[i]);
    }
  }
  output = "";
}

//*********************************************************
// 計算三軸坡度值
//*********************************************************
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
  PWM = 4*(PWMValue(bikeSpeed, gySlope))/5 + (reward(bikeSpeed))/30 + prePWM/15;
  prePWM = PWM;
  if(PWM<0) PWM = 0.0;
  if(PWM>1) PWM = 1.0;
}
