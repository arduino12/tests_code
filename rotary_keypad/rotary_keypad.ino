#include "Keyboard.h"

#define DIAL_PIN			(2)
#define DIAL_TIMEOUT		(100)
#define STATE_TIMEOUT		(10)

bool curState		= 0;
bool oldState		= 0;
uint8_t count		= 0;
int32_t oldMillis	= 0;

void setup() {
	pinMode(DIAL_PIN, INPUT_PULLUP);
	Keyboard.begin();
}

void loop() {
	int32_t deltaMillis = millis() - oldMillis;
	
	if (oldState ^ digitalRead(DIAL_PIN)) {
		oldState = !oldState;
		oldMillis = millis();
	}
	else if (deltaMillis > STATE_TIMEOUT && curState ^ oldState) {
		curState = oldState;
		if (curState) {
			if (count++) Keyboard.write(8);
			Keyboard.write('0' + count % 10);
		}
	}
	else if (deltaMillis > DIAL_TIMEOUT && count) {
		count = 0;
	}
}
