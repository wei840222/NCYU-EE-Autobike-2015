#include <Hall.h>
Hall h1;

void hall_1(){
  h1.stateUpdate();
}

void setup() {
  h1.set(2, 10);
  attachInterrupt(0, hall_1, FALLING);
}

void loop() {
  
  Serial.print(h1.getSpeed());
  Serial.print(" (km/hr)\t");
  Serial.print(h1.getAcc());
  Serial.println("(m/s**2)");

  delay(1000);  
}


