//***************************************************************
//裝置測試
//***************************************************************
void testDrives() {
  do {
    if (!Serial.available()) {
      LCD1602.print("BT connect failed");
      delay(1000);
      LCD1602.clear();
    }
  } while (!Serial.available());
  do {
    if (!GY521.testConnection()) {
      LCD1602.print("GY521 connect failed");
      delay(1000);
      LCD1602.clear();
      BT.write("#GY521 connect failed\n");
    }
  } while (!GY521.testConnection());
  LCD1602.print("All drives are ready!");
  delay(1500);
  LCD1602.clear();
  BT.write("#All drives are ready!\n");
}
//***************************************************************
//更新函式
//***************************************************************
void drivesUpdate() {
  gySlope = getAngleY();
  gyAcceleration = H2.getAcc();
}

void showLCD() {
  LCD1602.clear();
  LCD1602.print("Slope: ");
  LCD1602.print((int)gySlope);
  delay(1000);
}

void syncBT() {
  String output;
  output += (String)H2.getSpeed();
  output += ":";
  output += (String)gySlope;
  output += "\n";
  BT.write(output);
}

//***************************************************************
//計算Y軸角度值
//***************************************************************
//單位SI制
double getAngleY() {
  int16_t ax, ay, az;
  double Vax_offset = gyAcceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = gyAcceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}
