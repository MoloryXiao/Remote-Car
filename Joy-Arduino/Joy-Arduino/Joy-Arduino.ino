#include <OLED_CAR.h>
#include "Joystick.h"
#include "OledKeyDealWith.h"
#include "BlueTHSignal.h"

OLED_SPI MyOLED(13,12,11,10,9);
void setup() {
  // baud=9600 8data noCheck 2stop 
  Serial.begin(9600,SERIAL_8N2);  //蓝牙串口
  JoyStickPortInit();       //手柄端口初始化
  MyOLED.Init();            //包含了对OLED引脚的初始化
  OLED_FirstPrint(MyOLED);  //显示初始化
}
 
void loop() {
  if(SIGN_Flag) SignalDealWith();             //BlueTHSignal.h
  if(OLED_Flag) OLED_DealWithKeys(MyOLED);  //OledKeyDealWith.h
  if(ResetState())
  {
    OLED_Flag=1; SIGN_Flag=0;
    SignalMainSwitch(0);
    OLED_FirstPrint(MyOLED);    //显示初始化
  }
}
