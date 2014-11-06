#include <arduino/Arduino.h>
#include <Wire.h>

byte x = 0;

int main(){
  
  init();
  Wire.begin(); // join i2c bus (address optional for master)


  while(1){
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write("x is ");        // sends five bytes
    Wire.write(x);              // sends one byte  
    Wire.endTransmission();    // stop transmitting

    x=x+10;
    delay(100);
  }
}