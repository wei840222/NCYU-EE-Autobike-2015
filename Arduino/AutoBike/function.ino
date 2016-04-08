//***************************************************************
//裝置測試
//***************************************************************
void testDrives() {
  while (!Serial.available()) {
    LCD1602.print("BT connect failed");
    delay(1000);
    LCD1602.clear();
  }
  while (!GY521.testConnection()) {
    LCD1602.print("GY521 connect failed!");
    delay(1000);
    LCD1602.clear();
  }
  LCD1602.print("All drives are ready!");
  delay(1500);
  LCD1602.clear();
}
//***************************************************************
//更新函式
//***************************************************************
void drivesUpdate() {
  gySlope = getAngleY();
  hallAcceleration = H2.getAcc();
  hallSpeed = H2.getSpeed();
}

void showLCD() {
  LCD1602.clear();
  LCD1602.print("Slope: ");
  LCD1602.print((int)gySlope);
  delay(200);
}

void syncBT() {
  String output;
  output += (String)hallSpeed;
  output += ":";
  output += (String)gySlope;
  BT.write(output);
  mode = BT.read();
}

//***************************************************************
//計算Y軸角度值
//***************************************************************
//單位SI制
double getAngleY() {
  int16_t ax, ay, az;
  double Vax_offset = hallAcceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = hallAcceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}
