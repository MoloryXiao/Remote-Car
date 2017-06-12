#include "MotorControl.h"
#include "ClockDisplay.h"
#include "Buzz.h"
#include "BlueTHSignal.h"

#define COLLISION_PIN 3   //碰撞开关引脚

//外部碰撞开关中断
void CollisionISR()
{
  if(digitalRead(COLLISION_PIN)==HIGH) upRunFlag=1;
  else upRunFlag=0;
}
//Timer1 10ms中断
void TimerISR()
{
  TimingISR();
  BuzzISR();
}
void setup() {
  Serial.begin(9600,SERIAL_8N2);  //蓝牙端口
  MotorPortInit();    //电机端口初始化
  Tm1637Init();       //秒表初始化
  BuzzInit();     //蜂鸣器初始化
  //定时器初始化
  Timer1.initialize(10000);//timing for 10ms
  Timer1.attachInterrupt(TimerISR);
  //碰撞开关初始化
  pinMode(COLLISION_PIN,INPUT_PULLUP);
  attachInterrupt(1,CollisionISR,CHANGE);
}
void loop() {
  SignalDealWith();             //蓝牙信号接收处理 (BlueTHSignal.h)
  if(clockFlag && !mainSwitch)  //数码管使能 但是小车启动开关还没开
  {
    //闪烁的眼睛
  }
  if(mainSwitch)
  {
    if(clockFlag) ClockShow();  //秒表计数         (ClockDisplay.h)
    if(singFlag) SingASong();   //蜂鸣器唱歌       (Buzz.h)
  }
}

