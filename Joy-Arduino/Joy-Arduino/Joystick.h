#ifndef __JOYSTICK_H
#define __JOYSTICK_H	  

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define ANALOG_X_PIN 0
#define ANALOG_Y_PIN 1
#define BUTTON_UP_PIN 2
#define BUTTON_RIGHT_PIN 3
#define BUTTON_DOWN_PIN 4
#define BUTTON_LEFT_PIN 5
#define BUTTON_E_PIN 6
#define BUTTON_F_PIN 7

void JoyStickPortInit()
{
	pinMode(BUTTON_UP_PIN, INPUT);
	pinMode(BUTTON_RIGHT_PIN, INPUT);
	pinMode(BUTTON_DOWN_PIN, INPUT);
	pinMode(BUTTON_LEFT_PIN, INPUT);
	pinMode(BUTTON_E_PIN, INPUT);
	pinMode(BUTTON_F_PIN, INPUT);
}
#endif

/*
	if(digitalRead(BUTTON_UP) == LOW) {
	 Serial.write(WR_UP);
	 delay(DELAY);
	}
	if(digitalRead(BUTTON_RIGHT) == LOW) {
	 Serial.write(WR_RIGHT);
	 delay(DELAY);
	}
	if(digitalRead(BUTTON_DOWN) == LOW) {
	 Serial.write(WR_DOWN);
	 delay(DELAY);
	}
	if(digitalRead(BUTTON_LEFT) == LOW) {
	 Serial.write(WR_LEFT);
	 delay(DELAY);
	}
	if(digitalRead(BUTTON_E) == LOW) {
	 Serial.println("Button E is pressed");
	 delay(DELAY);
	}
	if(digitalRead(BUTTON_F) == LOW) {
	 Serial.println("Button F is pressed");
	 delay(DELAY);
	}
	X = analogRead(PIN_ANALOG_X);
	Y = analogRead(PIN_ANALOG_Y);

*/