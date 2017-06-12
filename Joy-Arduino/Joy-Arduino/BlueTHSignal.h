#ifndef __BLUETHSIGNAL_H
#define __BLUETHSIGNAL_H
#include "Joystick.h"
#include "PointAnalysis.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define WR_UP 		  0x01
#define WR_DOWN 	  0x02
#define WR_LEFT 	  0x03
#define WR_RIGHT 	  0x04

#define WR_MAIN  	  0xBB	//0xBB-BC
#define WR_BUZZ 	  0x88 	//0x88-8A
#define WR_SPEED 	  0x80	//0x80-0x87
#define WR_CLOCK	  0x90 	//0x90-91
#define SEND_DELAY 	  40

void SignalDealWith()
{
	if(digitalRead(BUTTON_UP_PIN) == LOW)
	{
		Serial.write(0x80|0x01);
		delay(SEND_DELAY);
	}

	Point p;
	unsigned char speedGet=0,direcGet=0;
	unsigned char signalSend;
	p.x=analogRead(ANALOG_X_PIN);
	p.y=analogRead(ANALOG_Y_PIN);
	speedGet=SpeedGet(p);
	direcGet=DirectionGet(p);
	signalSend |= direcGet<<4 | speedGet;
	if(speedGet)
	{
	  Serial.write(signalSend);
	  delay(SEND_DELAY);
	}
}
void SignalTest()
{
	unsigned char i,j;
	unsigned char speed=2;
	unsigned char direct[]={3,4,1,2};

	for(i=0;i<4;i++)
	{
		for(j=0;j<12;j++)
		{
			Serial.write(direct[i]<<4 | speed);
			delay(SEND_DELAY);
		}
		Serial.write(WR_BUZZ);
		if(i<3) delay(300);
		else delay(1000);
	}
	Serial.write(WR_BUZZ);
	delay(200);
	Serial.write(WR_BUZZ);
}
void SignalMusic(unsigned char state)
{
	if(state) Serial.write(WR_BUZZ+1);		//蜂鸣器打开
	else Serial.write(WR_BUZZ+2);
	delay(SEND_DELAY);
}
void SignalClock(unsigned char state)
{
	if(state) Serial.write(WR_CLOCK+0);
	else Serial.write(WR_CLOCK+1);
	delay(SEND_DELAY);
}
void SignalMainSwitch(unsigned char state)
{
	if(state) Serial.write(WR_MAIN+0);
	else Serial.write(WR_MAIN+1);
	delay(SEND_DELAY);
}
void SignalSpeed(unsigned char value)
{
	Serial.write(WR_SPEED+value);
	delay(SEND_DELAY);
}
#endif