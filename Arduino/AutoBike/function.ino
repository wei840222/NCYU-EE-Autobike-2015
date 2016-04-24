//*********************************************************
// 裝置測試
//*********************************************************
void testDrives() {
  while(!Serial.available()) {
    LCD1602.print("BT connect failed");
    delay(1000);
    LCD1602.clear();
  }
  while(!GY521.testConnection()) {
    LCD1602.print("GY521 connect failed!");
    delay(1000);
    LCD1602.clear();
  }
  LCD1602.print("All drives are ready!");
  delay(1500);
  LCD1602.clear();
}

//*********************************************************
// 更新函式
//*********************************************************
void drivesUpdate() {
  gySlope = getAngleY();
  hallAcceleration = Wheel.getAcc();
  pedalPower = getPedalPower();
}

void showLCD() {
  LCD1602.clear();
  LCD1602.print("Slope: ");
  LCD1602.print((int)gySlope);
}

void syncBT() {
  if(BT.read()=="$A\n") autoMode = !autoMode;
  LCD1602.setCursor(0, 1);
  LCD1602.print(BT.read());
  String output;
  output += (String)rpm;
  output += ":";
  output +=(String)gySlope;
  BT.write(output);
}

//*********************************************************
// 計算Y軸角度值
//*********************************************************
//單位SI制
double getAngleY() {
  int16_t ax, ay, az;
  double Vax_offset = hallAcceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = hallAcceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}

//*********************************************************
// 計算腳踏力量
//*********************************************************
double getPedalPower() {
  double alpha = Gear.getAcc();
  double I = gear_R*gear_R*gear_m/2/10000;
  return I*alpha;
}

//*********************************************************
// rpm
//*********************************************************
void updateRPM(){
  rpm = rpm_times/gear_magnetN;
  rpm_times = 0;
}

//*********************************************************
// PWM 輸出
//*********************************************************
void PWMInitialze() {
  pinMode(pin_pwm_output, OUTPUT);
}
void PWMOutput() {
  if(abs(gySlope) <= 5){
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)pedalPower/50*128-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 128);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    /*
    if(pedalPower < 50) {
      analogWrite(pin_pwm_output, 75);
    }else if(pedalPower < 30) {
    analogWrite(pin_pwm_output, 105);
    }else if(pedalPower < 15) {
      analogWrite(pin_pwm_output, 135);
    }else if(pedalPower == 0) {
      analogWrite(pin_pwm_output, 165);
    }
    */
  }else if(abs(gySlope) <= 15) {
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)pedalPower/50*192-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 192);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    /*
    if(pedalPower < 50) {
      analogWrite(pin_pwm_output, 105);
    }else if(pedalPower < 30) {
      analogWrite(pin_pwm_output, 135);
    }else if(pedalPower < 15) {
      analogWrite(pin_pwm_output, 165);
    }else if(pedalPower == 0) {
      analogWrite(pin_pwm_output, 195);
    }
    */
  }else if(abs(gySlope) <= 30) {
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)pedalPower/50*224-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 224);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    /*
    if(pedalPower < 50) {
      analogWrite(pin_pwm_output, 135);
    }else if(pedalPower < 30) {
      analogWrite(pin_pwm_output, 165);
    }else if(pedalPower < 15) {
      analogWrite(pin_pwm_output, 195);
    }else if(pedalPower == 0) {
      analogWrite(pin_pwm_output, 225);
    }
    */
  }else if(abs(gySlope) > 30) {
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)pedalPower/50*255-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 255);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    /*
    if(pedalPower < 50) {
      analogWrite(pin_pwm_output, 165);
    }else if(pedalPower < 30) {
      analogWrite(pin_pwm_output, 195);
    }else if(pedalPower < 15) {
      analogWrite(pin_pwm_output, 225);
    }else if(pedalPower == 0) {
      analogWrite(pin_pwm_output, 255);
    }
    */
  }
}
