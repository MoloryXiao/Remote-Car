#ifndef __CLOCKDISPLAY_H
#define __CLOCKDISPLAY_H	 

#include <TimerOne.h>
#include "TM1637.h"
#include "Buzz.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#define ON 1
#define OFF 0

#define TM_CLK 10
#define TM_DIO 11
#define BLINKFREQ 7   //中间的时间点的闪烁频率 单位:百毫秒

TM1637 tm1637(TM_CLK,TM_DIO);

int8_t timeDisp[] = {0x00,0x00,0x00,0x00};
unsigned int  blinkCount=0;
unsigned char clockPoint = 1;
unsigned char Update;
unsigned char second=0;
unsigned char minute = 0;
unsigned char hour = 0;
unsigned char secondTenmill=0;
unsigned char secondHunmill=0;

//tm1637初始化
void Tm1637Init()
{
  tm1637.set();
  tm1637.init();
}

//计时器中断后进行数字位计算
void TimingISR()
{
  secondTenmill ++;
  if(secondTenmill == 10){
    secondTenmill = 0;
    secondHunmill ++;
    if(secondHunmill==10){
      secondHunmill = 0;  
      second ++;
      if(second == 60){
        second = 0;
        minute ++;
        if(minute == 60){
          minute = 0;
          hour ++;
          if(hour == 24)hour = 0;
        }
      }
    }
  }
  //时钟点设置
  blinkCount++;
  if(blinkCount==BLINKFREQ*10) 
  {
    blinkCount=0;
    clockPoint = (~clockPoint) & 0x01;
  }
  //刷新Flag
  Update = ON;
}

//更新数码管显示
void TimeUpdate(void)
{
  if(clockPoint) tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
  if(minute==0)
  {
    timeDisp[0] = second / 10;
    timeDisp[1] = second % 10;
    timeDisp[2] = secondHunmill;
    timeDisp[3] = secondTenmill;
  }else
  {
    timeDisp[0] = minute / 10;
    timeDisp[1] = minute % 10;
    timeDisp[2] = second / 10;
    timeDisp[3] = second % 10;
  }
  Update = OFF;
}

//时间显示
void ClockShow()
{
  if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(timeDisp);
  }
}
#endif