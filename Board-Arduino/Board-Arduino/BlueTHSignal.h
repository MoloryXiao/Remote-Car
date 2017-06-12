#ifndef __BLUETHSIGNAL_H
#define __BLUETHSIGNAL_H	 
#include "MotorControl.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//蓝牙信号
#define RD_UP         0x01
#define RD_DOWN       0x02
#define RD_LEFT       0x03
#define RD_RIGHT      0x04

#define RD_SPEED_1      0x81
#define RD_SPEED_2      0x82
#define RD_SPEED_3      0x83
#define RD_SPEED_4      0x84
#define RD_SPEED_5      0x85
#define RD_BUZZ_TEST    0x88
#define RD_BUZZ_OPEN    0x89
#define RD_BUZZ_CLOSE   0x8A
#define RD_CLOCK_OPEN   0x90
#define RD_CLOCK_CLOSE  0x91
#define RD_MAIN_OPEN    0xBB
#define RD_MAIN_CLOSE   0xBC

unsigned char incrementSpeed=2;		//速度原始增量
unsigned char upRunFlag=1;        //前行使能
unsigned char mainSwitch=0;       //总开关
unsigned char clockFlag=0;        //时钟开关
unsigned char singFlag=0;         //唱歌开关

void SignalDealWith()
{
  unsigned char signalGet=0;
  unsigned char speedGet,direcGet,commandGet;
  signalGet = Serial.read();

  //没有收到数据时 signalGet为0xFF 为了不干扰命令信号 将最高位清零
  if(signalGet == 0xff) signalGet=0x7F;
  if(signalGet&0x80)        //判断是否为命令指令
  {
    Serial.println(signalGet);
    Serial.println("command");
    commandGet = signalGet;
    switch(commandGet)
    {
      case RD_SPEED_1: incrementSpeed=2; break;
      case RD_SPEED_2: incrementSpeed=3; break;
      case RD_SPEED_3: incrementSpeed=4; break;
      case RD_SPEED_4: incrementSpeed=5; break;
      case RD_SPEED_5: incrementSpeed=6; break;
      case RD_MAIN_OPEN:  mainSwitch=1;  break;
      case RD_MAIN_CLOSE: mainSwitch=0;  break;
      case RD_BUZZ_TEST:  BuzzTest();    break;
      case RD_BUZZ_OPEN:  singFlag=1;    break;
      case RD_BUZZ_CLOSE: singFlag=0;    break;
      case RD_CLOCK_OPEN: clockFlag=1;   break;
      case RD_CLOCK_CLOSE:clockFlag=0;   break;
    }
  }else     //最高位不是1，则是方向速度指令
  {
    direcGet = signalGet>>4;
    speedGet = signalGet&0x0F;
    speedGet += incrementSpeed;
    switch(direcGet)
    {
      case RD_UP:    if(upRunFlag) UpRun(speedGet);  break;
      case RD_DOWN:  DownRun(speedGet);  break;
      case RD_LEFT:  LeftRun(speedGet);  break;
      case RD_RIGHT: RightRun(speedGet); break;
      default: StopRun(); break;
    }
  }
}

#endif