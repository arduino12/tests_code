#include <Arduino.h>
#include "globals.h"

extern Screen screen;
extern Joystick joystick;
//extern View converter;

uint8_t standbyBegin() {
	
	//screen.drawBuffer(0,0,converter.beginDigit(4));
	return 0;
}

uint8_t standbyLoop() {
	
	
	return 0;
}

void standbyEnd() {
	//screen.drawBuffer(0,0,converter.beginDigit(4));
}
