//*********************************************************
// 裝置測試
//*********************************************************
void testDrives() {
  /*
  while(!Serial.available()) {
    LCD1602.print("BT connect failed");
    delay(1000);
    LCD1602.clear();
  }
  */
  while(!GY521.testConnection()) {
    LCD1602.print("GY521 connect failed!");
    delay(1000);
    LCD1602.clear();
  }
  /*
  LCD1602.print("All drives are ready!");
  delay(1500);
  LCD1602.clear();
  */
}

//*********************************************************
// 更新函式
//*********************************************************
void drivesUpdate() {
  // update 單車的加速度
  hallAcceleration = Wheel.getAcc();
  // update 單車的角度
  gySlope = getAngleY();
  // update 腳踏力道
  pedalPower = getPedalPower();
}

void showLCD() {
  // line 0
  LCD1602.clear();
  LCD1602.print("Ac: ");
  LCD1602.print((float)hallAcceleration, 10);
  
  // line 1
  LCD1602.setCursor(0, 1);
  LCD1602.print("V: ");
  LCD1602.print((float)Wheel.getSpeed(), 4);
  LCD1602.print("  S: ");
  LCD1602.print((int)gySlope);
  
  // end
  delay(250);
}

void syncBT() {
  //
  if(BT.read()=="$A\n") autoMode = !autoMode;
  //
  LCD1602.clear();
  LCD1602.setCursor(0, 1);
  LCD1602.print(BT.read());
  //
  String output;
  output += (String)rpm;
  output += ":";
  output +=(String)gySlope;
  BT.write(output);
  //
  delay(250);
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
  return I*alpha;
}

//*********************************************************
// rpm
//*********************************************************
void updateRPM(){
  rpm = rpm_ttimes/gear_magnetN;
  rpm_ttimes = 0;
}

//*********************************************************
// PWM 輸出
//*********************************************************
void PWMOutput() {
  if(abs(gySlope) <= 5){
    if(pedalPower < pedalPower_MAX && pedalPower >= pedalPower_MIN) {
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*128)-1);
    }else if(pedalPower >= pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
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
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*192)-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
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
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*224)-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
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
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*255)-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
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
