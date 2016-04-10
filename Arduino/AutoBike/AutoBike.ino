//***************************************************************
//載入函式庫
//***************************************************************
#include <LiquidCrystal.h>
#include "MPU6050.h"
#include "Hall.h"
#include "HC05.h"
#include "Timer.h"

//***************************************************************
//定義腳位
//***************************************************************
#define pin_bluetooth_RXD 0
#define pin_bluetooth_TXD 1
#define pin_hall_1 2
#define pin_hall_2 3
#define pin_lcd_RS 9
//LCD R/W 要接地
#define pin_lcd_E 8
#define pin_lcd_D4 7
#define pin_lcd_D5 6
#define pin_lcd_D6 5
#define pin_lcd_D7 4
#define pin_pwm_output 10
#define pin_sda A4
#define pin_scl A5

//**********************************************************
//常數
//**********************************************************

const int baudrate = 9600;  //  bps
const int gear_magnetN = 3;
const double gear_R = 10;   //  cm
const double gear_m = 2;   //  kg
const double wheel_R = 29;  //  cm
const double pedalPower_MAX = 50;
const double pedalPower_MIN = 5;

//**********************************************************
//全域變數
//**********************************************************

boolean autoMode = 1;
boolean pwmSwitch = 1;
int rpm_times = 0;
int rpm = 0;
double gySlope = 0;         //  degree
double hallAcceleration = 0;  //  N/s^2
double pedalPower = 0;

//***************************************************************
//建立裝置物件
//***************************************************************
LiquidCrystal LCD1602(pin_lcd_RS, pin_lcd_E, pin_lcd_D4, pin_lcd_D5, pin_lcd_D6, pin_lcd_D7);
MPU6050 GY521;
Hall Gear(pin_hall_1, gear_R), Wheel(pin_hall_2, wheel_R);
HC05 BT(baudrate);  //包含初始化
Timer T1;  //計算RPM用

//***************************************************************
//初始化設定
//***************************************************************
void setup() {
  //設定baudrate
  Serial.begin(baudrate);
  //霍爾感測器中斷初始化
  //踏板
  attachInterrupt(0, ISR_0, FALLING);
  //後輪
  attachInterrupt(1, ISR_1, FALLING);
  //初始化LCD
  LCD1602.begin(16, 2);
  LCD1602.print("LED is OK!");
  delay(1000);
  LCD1602.clear();
  //計算rpm初始化
  T1.every(60000, updateRPM);
  //初始化GY-521
  GY521.initialize();
  //初始化output
  PWMInitialze();
  //連線檢查
  //testDrives();
}

//***************************************************************
//主迴圈
//***************************************************************
void loop() {
  drivesUpdate();

  T1.update();
  if(autoMode){
    if(Wheel.getSpeed() >= 25) {
      pwmSwitch = 0;
    }else if(Wheel.getSpeed() < 25) {
      pwmSwitch = 1;
    }
  }else {
    pwmSwitch = 0;
  }
  if(pwmSwitch) PWMOutput();
  showLCD();
  syncBT();
  delay(250);
}

//***************************************************************
//中斷副程式 for 霍爾感應器
//***************************************************************
void ISR_0() {
  Gear.stateUpdate();
  rpm_times++;
}

void ISR_1() {
  Wheel.stateUpdate();
}
