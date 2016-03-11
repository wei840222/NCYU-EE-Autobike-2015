#include <Wire.h>
#include <LiquidCrystal.h>

#include "I2Cdev.h"
#include "MPU6050.h"

//全域變數
double Slope = 0;
double Acceleration = 0;

//建立裝置物件
MPU6050 GY521;
LiquidCrystal LCD1602(7, 6, 5, 4, 3, 2);

/////////////////////// GY-521_Functions ///////////////////////
double GetAngleY(){  //計算Y軸角度值
  int16_t ax, ay, az;
  double Vax_offset = Acceleration * sin(Slope) * 16384 / 9.8;
  double Vay_offset = Acceleration * cos(Slope) * 16384 / 9.8;
  GY521.getAcceleration(&ax, &ay, &az);
  return 60 * atan((ay - Vay_offset) / sqrt(pow(ax - Vax_offset, 2) + pow(az, 2)));
}

/////////////////////// HC-05_Functions ///////////////////////
String HC05_GetString(){  //從藍芽接收字串
  byte BTbuffer;
  String BTstring;
  if(Serial.available()){
    while(Serial.available()){
      BTbuffer = Serial.read();
      BTstring += (char)BTbuffer;
    }
  }
  return BTstring;
}

void HC05_SendString(String BTstring){  //從藍芽送出字串
  byte BTbuffer[32];
  BTstring.getBytes(BTbuffer, BTstring.length());
  if(Serial.available())
    for(int i = 0;i < BTstring.length();i++)
      Serial.write(BTbuffer[i]);
}

void setup(){
  //藍芽HC-05傳輸速率
  Serial.begin(9600);

  //設定LCD游標值
  LCD1602.begin(16, 2);
    
  //初始化GY-521
  GY521.initialize();

  //GY-521連線檢查
  do{
    if(!GY521.testConnection()){
      LCD1602.print("connect failed");
      delay(1000);
      LCD1602.clear();
    }
  }while(!GY521.testConnection());

  //歡迎訊息
  LCD1602.print("Hello!");
  delay(1000);
  LCD1602.clear();
}


void loop(){
  //取得角度值
  Slope = GetAngleY();
  //顯示
  LCD1602.print("Slope:");
  LCD1602.print((int)Slope);
  LCD1602.setCursor(0, 1);
  LCD1602.print("BTMsg:");
  LCD1602.print(HC05_GetString());
  delay(1000);
  LCD1602.clear();
}
