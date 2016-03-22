//***************************************************************
//載入函式庫*
//***************************************************************
#include <Wire.h>
#include <LiquidCrystal.h>

#include "MPU6050.h"
#include "Hall.h"
#include "HC05.h"
//***************************************************************
//***************************************************************
//***************************************************************
#define pin_bluetooth_RXD 0
#define pin_bluetooth_TXD 1
#define pin_hall_1 2
#define pin_hall_2 3
#define pin_lcd_RS 9
#define pin_lcd_E 8 
#define pin_lcd_D4 7
#define pin_lcd_D5 6 
#define pin_lcd_D6 5
#define pin_lcd_D7 4
#define pin_pwm_output 10
#define pin_sda A4
#define pin_scl A5

//***************************************************************
//常數,全域變數
//***************************************************************
const int baudrate = 9600;
const double gear_R = 10;
const double wheel_R = 29;
double gySlope = 0;
double gyAcceleration = 0;

//***************************************************************
//建立裝置物件
//***************************************************************
LiquidCrystal LCD1602(pin_lcd_RS, pin_lcd_E, pin_lcd_D4, pin_lcd_D5, pin_lcd_D6, pin_lcd_D7);
MPU6050 GY521;
Hall H1, H2;
HC05 BT(baudrate);

//***************************************************************
//初始化設定
//***************************************************************
void setup(){
  //霍爾感測器初始化
  H1.set(pin_hall_1, gear_R);  //位於踏板附近
  attachInterrupt(0, ISR_0, FALLING);
  H2.set(pin_hall_2, wheel_R);  //位於後輪附近
  attachInterrupt(0, ISR_1, FALLING);
  
  //初始化LCD
  LCD1602.begin(16, 2);
  LCD1602.autoscroll();
  //初始化GY-521
  GY521.initialize();
  
  //連線檢查
  testAllDrives();
}

//***************************************************************
//更新
//***************************************************************
void loop(){
  drivesUpdate();
  show();
}

//***************************************************************
//中斷副程式 for 霍爾感應器
//***************************************************************
void ISR_0(){
  H1.stateUpdate();
}

void ISR_1(){
  H2.stateUpdate();
}
//***************************************************************
//
//***************************************************************
void testAllDrives() {
  do {
     if(!Serial.available()){
      LCD1602.print("BT connect failed");
      delay(1000);
      LCD1602.clear();
     }
  }while(!Serial.available()); 
  do {
    if(!GY521.testConnection()){
      LCD1602.print("GY521 connect failed");
      delay(1000);
      LCD1602.clear();
      BT.write("#GY521 connect failed\n");
    }
  }while(!GY521.testConnection());
  LCD1602.print("All drives are ready!");
  delay(1500);
  LCD1602.clear();
  BT.write("#All drives are ready!\n");
}
//***************************************************************
//更新函式
//***************************************************************
void drivesUpdate() {
  gySlope = GetAngleY();
  gyAcceleration = H2.getAcc();
}

void show() {
  LCD1602.print("Slope: ");
  LCD1602.print((int)gySlope);
  delay(1000);
  LCD1602.clear();
  String str;
  str += (String)H2.getSpeed();
  str += ":";
  str += (String)gySlope;
  str += "\n";
  BT.write(str);
}

//***************************************************************
/////////////////////// GY-521_Functions ///////////////////////
//***************************************************************
double GetAngleY(){  //計算Y軸角度值
  int16_t ax, ay, az;
  double Vax_offset = gyAcceleration * sin(gySlope) * 16384 / 9.8;
  double Vay_offset = gyAcceleration * cos(gySlope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}
