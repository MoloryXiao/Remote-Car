#ifndef __OLEDKEYDEALWITH_H
#define __OLEDKEYDEALWITH_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Joystick.h"
#include "BlueTHSignal.h"

#define MAXROW 8        //选项数
#define MINROW 1
#define SHAKE_DELAY_1 250  //消抖延时
#define SHAKE_DELAY_2 100

unsigned char checkedRowNum=1; //初始化选中行为第一行(第0行为title)
unsigned char menuPoint=1;	   //滑动窗口头指针
//选项选中配置 1为选中 0位不选中
unsigned char checkedState[9]={0,1,0,0,0,0,0,0,0};
//选项居中配置 1为居中 0为左对齐
unsigned char middleState[9]={1,1,1,1,1,1,1,1,1}; 

unsigned char SIGN_Flag=0;
unsigned char OLED_Flag=1;
unsigned char musicOpenFlag=0;
unsigned char clockOpenFlag=0;
unsigned char speedParam=1;
//  选项结构体
struct Menu_Item
{
  char content[17];
};
const struct Menu_Item EN_Menu[]=
{
  {"PARAM MENU"},
  {"CAR START"},
  {"BUZZ STATE"},
  {"CLOCK STATE"},
  {"SPEED ADJ"},
  {"CAR TEST"},
  {"LEFT-CIR TEST"},
  {"RIGHT-CIR TEST"},
  {"ABOUT ME"}
};
void OLED_FirstPrint(OLED_SPI &MyOLED)
{
  unsigned char i;
  //初始化显示内容
  for(i=0;i<4;i++)
  {
    MyOLED.Print_ENString(0,i*2,EN_Menu[i].content,checkedState[i],middleState[i]);
  }
}
void OLED_ReturnPrint(OLED_SPI &MyOLED)
{
  unsigned char i,row;
  MyOLED.Clear();
  MyOLED.Print_ENString(0,0,EN_Menu[0].content,checkedState[0],middleState[0]);
  for(i=0;i<3;i++)
  {
    row=menuPoint+i;
    MyOLED.Print_ENString(0,(i+1)*2,EN_Menu[row].content,checkedState[row],middleState[row]);
  }
}
bool PlusState()
{
  if(checkedRowNum+1<=MAXROW)
  {
    if(analogRead(ANALOG_Y_PIN)<412)
    {
      delay(SHAKE_DELAY_1);
      return true;
    }
    if(digitalRead(BUTTON_DOWN_PIN)==0) 
    {
      delay(SHAKE_DELAY_2);
      if(digitalRead(BUTTON_DOWN_PIN)==0) return true;
      else return false;
    }
    return false;
  }
  return false;
}
bool MinusState()
{
  if(checkedRowNum-1>=MINROW)
  {
    if(analogRead(ANALOG_Y_PIN)>612)
    {
      delay(SHAKE_DELAY_1);
      return true;
    }
    if(digitalRead(BUTTON_UP_PIN)==0)
    {
      delay(SHAKE_DELAY_2);
      if(digitalRead(BUTTON_UP_PIN)==0) return true;
      else return false;
    }
    return false;
  }
  return false;
}
bool EnterState()
{
  if(digitalRead(BUTTON_LEFT_PIN)==0)
  {
    delay(SHAKE_DELAY_2);
    if(digitalRead(BUTTON_LEFT_PIN)==0) return true;
    else return false;
  }else return false;
}
bool CancelState()
{
  if(digitalRead(BUTTON_RIGHT_PIN)==0)
  {
    delay(SHAKE_DELAY_2);
    if(digitalRead(BUTTON_RIGHT_PIN)==0) return true;
    else return false;
  }else return false;
}
bool ResetState()
{
  if(digitalRead(BUTTON_F_PIN)==0)
  {
    delay(SHAKE_DELAY_2);
    if(digitalRead(BUTTON_F_PIN)==0) return true;
    else return false;
  }else return false;
}
bool OK_Or_NO()
{
  while(1)
  {
    if(EnterState())
    {
      return 1;
    }
    if(CancelState())
    {
      return 0;
    }
  }
}
void SecondLevelMenu(OLED_SPI &MyOLED)
{
  unsigned char result,maxValue=0,pointCount=0,tempValue;
  unsigned char outFlag=0;
  switch(checkedRowNum)   //根据选项不同 进入不同的界面
  {
    case 1:         //小车启动确认
      MyOLED.Comfirm_Page(EN_Menu[checkedRowNum].content,"SURE TO ENTER?"); 
      result=OK_Or_NO();    //等待确认
      if(result)            //确认进入遥控模式
      {
        MyOLED.ENStr_Page("GAMING...");
        SignalMainSwitch(1);//发送打开Board端总开关的指令
        SIGN_Flag=1;        //Joy端进入只发送信号模式
        OLED_Flag=0;        //退出OLED屏调试模式
      }else                 //返回主菜单
        OLED_ReturnPrint(MyOLED);
    break;
    case 2:          //音乐开关选项
      MyOLED.Switch_Page(EN_Menu[checkedRowNum].content,"MUSIC:",musicOpenFlag,1);
      do       //左键反选，直到按下右键
      {
        if(EnterState()) 
        {
          if(musicOpenFlag) musicOpenFlag=0;
          else musicOpenFlag=1;
          MyOLED.Switch_Page(EN_Menu[checkedRowNum].content,"MUSIC:",musicOpenFlag,0);
        }
      }while(!CancelState());
      SignalMusic(musicOpenFlag);   //发送音乐状态的指令
      OLED_ReturnPrint(MyOLED);     //返回主菜单
    break;
    case 3:         //时钟开关选项
      MyOLED.Switch_Page(EN_Menu[checkedRowNum].content,"CLOCK:",clockOpenFlag,1);
      do       //左键反选，直到按下右键
      {
        if(EnterState()) 
        {
          if(clockOpenFlag) clockOpenFlag=0;
          else clockOpenFlag=1;
          MyOLED.Switch_Page(EN_Menu[checkedRowNum].content,"CLOCK:",clockOpenFlag,0);
        }
      }while(!CancelState());
      SignalClock(clockOpenFlag);   //发送音乐状态的指令
      OLED_ReturnPrint(MyOLED);     //返回主菜单
    break;
    case 4:
      maxValue=5;
      tempValue=speedParam;
      pointCount = 48*speedParam/maxValue;
      MyOLED.Adjust_Page(EN_Menu[checkedRowNum].content,tempValue,maxValue,1);
      do
      {
        if(MinusState() && tempValue+1<=maxValue)
        {
          tempValue += 1;
          pointCount = 48*tempValue/maxValue;
          MyOLED.Print_ALine(16,3,pointCount,1);
          MyOLED.Print_Num(0,6,tempValue,1,0,1);
        }
        if(PlusState() && tempValue-1>=1)
        {
          tempValue -= 1;
          pointCount = 48*tempValue/maxValue;
          MyOLED.Print_ALine(16+2*pointCount,3,48-pointCount,0);
          MyOLED.Print_Num(0,6,tempValue,1,0,1);
        }
        if(EnterState())
        {
          if(tempValue != speedParam) SignalSpeed(tempValue);
          speedParam = tempValue;
          outFlag=1;
        }
        if(CancelState()) outFlag = 1;
      }while(!outFlag);
      OLED_ReturnPrint(MyOLED);     //返回主菜单
      break;
    case 5:           //小车跑动测试选项
      MyOLED.Comfirm_Page(EN_Menu[checkedRowNum].content,"NOW TO TEST?");  
      result=OK_Or_NO();    //等待确认
      if(result)            //进入测试
      {
        MyOLED.ENStr_Page("CAR TEST...");
        SignalTest();       //发送小车跑动测试指令
        delay(500);
        MyOLED.ENStr_Page("TEST OVER!");
        delay(800);
      }
      OLED_ReturnPrint(MyOLED); //返回主菜单
    break;
  }
}
void OLED_DealWithKeys(OLED_SPI &MyOLED)
{
  unsigned char i,j;
  unsigned char selectRow,cancelRow;  //选中行,取消选中行
  if(PlusState())
  {
    checkedState[checkedRowNum]=0;  //原来项取消选中
    checkedRowNum++;
    checkedState[checkedRowNum]=1;  //新项选中

    if(menuPoint+2<checkedRowNum)
    {
      menuPoint++;
      for(i=menuPoint,j=1;i<menuPoint+3,j<4;i++,j++)
      {
        if(j!=3)  //减少不必要的刷新
          MyOLED.Dele_Row(j);
        //MyOLED.Print_CNString(0,j,i,checkedState[i],middleState[i]);
        MyOLED.Print_ENString(0,j*2,EN_Menu[i].content,checkedState[i],middleState[i]);
      }
    }else
    {
        cancelRow=checkedRowNum-menuPoint;
        i=checkedRowNum-1;
        MyOLED.Dele_Row(cancelRow);
        //MyOLED.Print_CNString(0,cancelRow,i,checkedState[i],middleState[i]);
        MyOLED.Print_ENString(0,cancelRow*2,EN_Menu[i].content,checkedState[i],middleState[i]);
        
        selectRow=checkedRowNum-menuPoint+1;
        i=checkedRowNum;
        //MyOLED.Dele_Row(selectRow);
        //MyOLED.Print_CNString(0,selectRow,i,checkedState[i],middleState[i]);
        MyOLED.Print_ENString(0,selectRow*2,EN_Menu[i].content,checkedState[i],middleState[i]);
    }
  }

  if(MinusState())
  {
    checkedState[checkedRowNum]=0;  //原来项取消选中
    checkedRowNum--;
    checkedState[checkedRowNum]=1;  //新项选中

    if(menuPoint>checkedRowNum) 
    {
      menuPoint--;
      for(i=menuPoint,j=1;i<menuPoint+3,j<4;i++,j++)
      {
        if(j!=1)  //减少不必要的刷新
          MyOLED.Dele_Row(j);
        //MyOLED.Print_CNString(0,j,i,checkedState[i],middleState[i]);
        MyOLED.Print_ENString(0,j*2,EN_Menu[i].content,checkedState[i],middleState[i]);
      }
    }else
    {
        selectRow=checkedRowNum-menuPoint+1;
        i=checkedRowNum;
        //MyOLED.Dele_Row(selectRow);
        //MyOLED.Print_CNString(0,selectRow,i,checkedState[i],middleState[i]);
        MyOLED.Print_ENString(0,selectRow*2,EN_Menu[i].content,checkedState[i],middleState[i]);
        
        cancelRow=checkedRowNum-menuPoint+2;
        i=checkedRowNum+1;
        MyOLED.Dele_Row(cancelRow);
        //MyOLED.Print_CNString(0,cancelRow,i,checkedState[i],middleState[i]);
        MyOLED.Print_ENString(0,cancelRow*2,EN_Menu[i].content,checkedState[i],middleState[i]);
    }
  }
  if(EnterState())
  {
    SecondLevelMenu(MyOLED);
  }
}
#endif  