//***************************************************************
//載入函式庫
//***************************************************************
#include <LiquidCrystal.h>
#include "MPU6050.h"
#include "Hall.h"
#include "HC05.h"

//***************************************************************
//定義腳位
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
//常數 and 全域變數
//***************************************************************
//請註解一下單位
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
Hall H1(pin_hall_1, gear_R), H2(pin_hall_2, wheel_R);
HC05 BT(baudrate);

//***************************************************************
//初始化設定
//***************************************************************
void setup() {
  //霍爾感測器中斷初始化
  //踏板
  attachInterrupt(0, ISR_0, FALLING);
  //後輪
  attachInterrupt(0, ISR_1, FALLING);

  //初始化LCD
  LCD1602.begin(16, 2);
  LCD1602.autoscroll();
  //初始化GY-521
  GY521.initialize();

  //連線檢查
  testDrives();
}

//***************************************************************
//主迴圈
//***************************************************************
void loop() {
  drivesUpdate();
  showLCD();
  syncBT();
}

//***************************************************************
//中斷副程式 for 霍爾感應器
//***************************************************************
void ISR_0() {
  H1.stateUpdate();
}

void ISR_1() {
  H2.stateUpdate();
}
