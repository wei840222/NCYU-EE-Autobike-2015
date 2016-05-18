//********************************************
// 載入函式庫
//********************************************
#include <LiquidCrystal.h>
#include "MPU6050.h"
#include "Hall.h"
#include "HC05.h"

//********************************************
// 定義腳位
//********************************************
#define pin_bluetooth_RXD 0
#define pin_bluetooth_TXD 1
#define pin_hall_1 2
#define pin_hall_2 3
#define pin_lcd_RS 9
// LCD R/W 要接地
#define pin_lcd_E 8
#define pin_lcd_D4 7
#define pin_lcd_D5 6
#define pin_lcd_D6 5
#define pin_lcd_D7 4
#define pin_pwm_output 10
#define pin_stop_anytime 12
#define pin_sda A4
#define pin_scl A5

//***************************************
// 常數
//***************************************
// for bluetooth
const int baudrate = 9600;   //  bps

// for hall 1: gear
const int gear_magnetN = 6;
const double gear_R = 0.105; // m
const double gear_m = 2;     // kg
//const double I = gear_R*gear_R*gear_m/2+2*crank_m*crank_l*crank_l/3;
const double I = gear_R*gear_R*gear_m/2;

// for hall 2: wheel
const int wheel_magnetN = 6;
const double wheel_R = 0.275; // m

// 曲柄
const double crank_m = 0.3;       // kg
const double crank_l = 0.175;     // m
// 功率上下限
const double pedalPower_MAX = 25;
const double pedalPower_MIN = 0;

//***************************************
// 全域變數
//***************************************
boolean autoMode = true;
boolean pwmSwitch = true;

double rps = 0;
double pre_rps = 0;
double rpm = 0;

double gySlope = 0;           //  degree
double bikeSpeed = 0;
double acceleration = 0;  //  m/s^2

double pedalTorque= 0;        //  N
double pre_pedalTorque = 0;
double pedalPower = 0;        //  N-m
double pre_pedalPower = 0;

int PWM;
//********************************************
// 建立裝置物件
//********************************************
LiquidCrystal LCD1602(pin_lcd_RS, pin_lcd_E, pin_lcd_D4, pin_lcd_D5, pin_lcd_D6, pin_lcd_D7);
MPU6050 GY521;
Hall Gear(pin_hall_1, gear_magnetN);
Hall Wheel(pin_hall_2, wheel_magnetN);
HC05 BT(baudrate);  //包含初始化BT

//********************************************
// 初始化設定
//********************************************
void setup() {
  //設定baudrate(arduino)
  Serial.begin(baudrate);
  //霍爾感測器中斷初始化
  //踏板
  attachInterrupt(0, ISR_0, FALLING);
  //後輪
  attachInterrupt(1, ISR_1, FALLING);
  //初始化LCD
  LCD1602.begin(16, 2);
  //初始化GY-521
  GY521.initialize();
  //初始化output
  pinMode(pin_pwm_output, OUTPUT);
  //初始化剎車按鈕
  pinMode(pin_stop_anytime, INPUT);
  //裝置測試
  //testGY521();
}

//********************************************
// 主迴圈
//********************************************
void loop() {
  //
  drivesUpdate();
  //
  if((millis()-Gear._nowTime)>5000) {
    Gear._preTime = 0;       //前一個時間點
    Gear._preOmega = 0;
    Gear._nowOmega = 0;
    Gear._nowAlpha = 0;
  }
  if((millis()-Wheel._nowTime)>5000) {
    Wheel._preTime = 0;       //前一個時間點
    Wheel._preOmega = 0;
    Wheel._nowOmega = 0;
    Wheel._nowAlpha = 0;
  }
  // 剎車功能

  //  if(!pin_stop_anytime) {
  //    autoMode = 0;
  //  }else {
  //    autoMode = 1;
  //  }

  // 助力模式 or 非助力模式
  if(autoMode){
    if(bikeSpeed >= 25) {
      pwmSwitch = 0;
    }else if(bikeSpeed < 25) {
      pwmSwitch = 1;
    }
  }else {
    pwmSwitch = 0;
  }
  // PWM輸出
  if(pwmSwitch) PWMOutput();
  // 顯示螢幕
  showLCD();
  // 與手機APP同步
  syncBT();
  // Serial.println(pedalPower);
  // Serial.println(pedalTorque);
  if(bikeSpeed>0 && bikeSpeed<15){
      Serial.println((int)abs(pedalPower/pedalPower_MAX*255+ abs(gySlope)));
  }else if(bikeSpeed>15){
      Serial.println((int)abs(pedalPower/pedalPower_MAX*(1-(int)(bikeSpeed-15)/9)*255+ abs(gySlope)));
  }
}

//***************************************
// 中斷副程式 for 霍爾感應器
//***************************************
void ISR_0() {
  Gear.stateUpdate();
}

void ISR_1() {
  Wheel.stateUpdate();
}
