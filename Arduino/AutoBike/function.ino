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
  // update 單車的速度
  bikeSpeed = Wheel.getOmega()*wheel_R;
  // update rps
  pre_rps = rps;
  rps = bikeSpeed/2/PI;
  // update 單車的加速度
  acceleration = Wheel.getAlpha()*wheel_R;
  // update 單車的角度
  gySlope = getAngleY();
  // update 腳踏力道
  pedalPower = getPedalPower();
}

void showLCD() {
  // line 0
  LCD1602.clear();
  LCD1602.print("P: ");
  LCD1602.print(pedalPower, 8);
  
  // line 1
  LCD1602.setCursor(0, 1);
  LCD1602.print("V: ");
  LCD1602.print(bikeSpeed, 4);
  LCD1602.print("  S: ");
  LCD1602.print((int)gySlope);
  
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
    Serial.print(output);
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
double getPedalPower() {
  double alpha = Gear.getAlpha();
  return I*alpha*rps*18;
}

//*********************************************************
// PWM 輸出
//*********************************************************
void PWMOutput() {
  if(abs(gySlope) <= 5){
    // if(pedalPower < pedalPower_MAX && pedalPower >= pedalPower_MIN) {
    //   analogWrite(pin_pwm_output, pedalPower/pedalPower_MAX*256-1);
    // }else if(pedalPower >= pedalPower_MAX) {
    //   analogWrite(pin_pwm_output, 0);
    // }else if(pedalPower < pedalPower_MIN) {
    //   analogWrite(pin_pwm_output, 0);
    // }
    if(bikeSpeed>0 && bikeSpeed<15) {
      analogWrite(pin_pwm_output, pedalPower/pedalPower_MAX*255);
    }else if(bikeSpeed>15 && bikeSpeed<24) {
      analogWrite(pin_pwm_output, pedalPower/pedalPower_MAX*(1-(int)(bikeSpeed-15)/9)*255);
    }else {
      
    }
    //  if(pedalPower < 50) {
    //    analogWrite(pin_pwm_output, 75);
    //  }else if(pedalPower < 30) {
    //  analogWrite(pin_pwm_output, 105);
    //  }else if(pedalPower < 15) {
    //    analogWrite(pin_pwm_output, 135);
    //  }else if(pedalPower == 0) {
    //    analogWrite(pin_pwm_output, 165);
    //  }
  }/*else if(abs(gySlope) <= 15) {
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*192)-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    
    //  if(pedalPower < 50) {
    //    analogWrite(pin_pwm_output, 105);
    //  }else if(pedalPower < 30) {
    //    analogWrite(pin_pwm_output, 135);
    //  }else if(pedalPower < 15) {
    //    analogWrite(pin_pwm_output, 165);
    //  }else if(pedalPower == 0) {
    //    analogWrite(pin_pwm_output, 195);
    //  }
    
  }else if(abs(gySlope) <= 30) {
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*224)-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    
    //  if(pedalPower < 50) {
    //    analogWrite(pin_pwm_output, 135);
    //  }else if(pedalPower < 30) {
    //    analogWrite(pin_pwm_output, 165);
    //  }else if(pedalPower < 15) {
    //    analogWrite(pin_pwm_output, 195);
    //  }else if(pedalPower == 0) {
    //    analogWrite(pin_pwm_output, 225);
    //  }
    
  }else if(abs(gySlope) > 30) {
    if(pedalPower <= pedalPower_MAX) {
      analogWrite(pin_pwm_output, (int)(pedalPower/pedalPower_MAX*255)-1);
    }else if(pedalPower > pedalPower_MAX) {
      analogWrite(pin_pwm_output, 0);
    }else if(pedalPower < pedalPower_MIN) {
      analogWrite(pin_pwm_output, 0);
    }
    
    //  if(pedalPower < 50) {
    //    analogWrite(pin_pwm_output, 165);
    //  }else if(pedalPower < 30) {
    //    analogWrite(pin_pwm_output, 195);
    //  }else if(pedalPower < 15) {
    //    analogWrite(pin_pwm_output, 225);
    //  }else if(pedalPower == 0) {
    //    analogWrite(pin_pwm_output, 255);
    //  }
    
  }*/
}