#ifndef __MOTORCONTROL_H
#define __MOTORCONTROL_H	  

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define ENA_PIN 9
#define ENB_PIN 4
#define INA1_PIN 8
#define INA2_PIN 7
#define INB1_PIN 6
#define INB2_PIN 5

#define SET_ENA() digitalWrite(ENA_PIN,HIGH)
#define CLR_ENA() digitalWrite(ENA_PIN,LOW)
#define SET_ENB() digitalWrite(ENB_PIN,HIGH)
#define CLR_ENB() digitalWrite(ENB_PIN,LOW)

#define SET_INA1() digitalWrite(INA1_PIN,HIGH)
#define CLR_INA1() digitalWrite(INA1_PIN,LOW)
#define SET_INA2() digitalWrite(INA2_PIN,HIGH)
#define CLR_INA2() digitalWrite(INA2_PIN,LOW)

#define SET_INB1() digitalWrite(INB1_PIN,HIGH) 
#define CLR_INB1() digitalWrite(INB1_PIN,LOW) 
#define SET_INB2() digitalWrite(INB2_PIN,HIGH)
#define CLR_INB2() digitalWrite(INB2_PIN,LOW)

void MotorPortInit()
{
    //电机端口
    pinMode(ENA_PIN,OUTPUT);pinMode(ENB_PIN,OUTPUT);
    pinMode(INA1_PIN,OUTPUT);pinMode(INA2_PIN,OUTPUT);
    pinMode(INB1_PIN,OUTPUT);pinMode(INB2_PIN,OUTPUT);
}

void UpRun(unsigned char speedGet)
{
    unsigned char i;
    //A轮 正转
    CLR_INA1();
    SET_INA2();
    //B轮 正转
    CLR_INB1();
    SET_INB2();
    for(i=0;i<5;i++)
    {
        if(i%2==0)
        {
            CLR_ENB();
            CLR_ENA();
            delay(10-speedGet);
        }else
        {
            SET_ENB();
            SET_ENA();
            delay(speedGet);
        }
    }
    CLR_ENB();
    CLR_ENA();
}
void DownRun(unsigned char speedGet)
{
    unsigned char i;
    //A轮 反转
    SET_INA1();
    CLR_INA2();
    //B轮 反转
    SET_INB1();
    CLR_INB2();
    
    for(i=0;i<5;i++)
    {
        if(i%2==0)
        {
            CLR_ENB();
            CLR_ENA();
            delay(10-speedGet);
        }else
        {   
            SET_ENB();
            SET_ENA();
            delay(speedGet);
        }
    }
    CLR_ENB();
    CLR_ENA();
}
void LeftRun(unsigned char speedGet)
{
    unsigned char i;
    //B轮 正转
    CLR_INB1();
    SET_INB2();
    SET_ENA();	//必须使能 否则电机不转动
    
    for(i=0;i<5;i++)
    {
        if(i%2==0)
        {
            CLR_ENB();
            delay(10-speedGet);
        }else
        {   
            SET_ENB();
            delay(speedGet);
        }
    }
    CLR_ENB();
    CLR_ENA();
}
void RightRun(unsigned char speedGet)
{
    unsigned char i;
    //A轮 正转
    CLR_INA1();
    SET_INA2();
    SET_ENB();	//必须使能 否则电机不转动
    for(i=0;i<5;i++)
    {
        if(i%2==0)
        {
            CLR_ENA();
            delay(10-speedGet);
        }else
        {
            SET_ENA();
            delay(speedGet);
        }
    }
    CLR_ENA();
    CLR_ENB();
}
void StopRun(void)
{
    CLR_ENA();
	CLR_INA1();
	CLR_INA2();

	CLR_ENB();	
	CLR_INB1();
	CLR_INB2();
}
#endif  
