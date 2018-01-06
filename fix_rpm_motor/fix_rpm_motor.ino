#include "motor.h"
#include "log.h"

#define BUTTON_PIN				(2)


void setup() {
	LOG_INIT;
	LOGLN(F("setup!"));

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	motor_setup();

	LOGLN(F("loop!"));
}

void loop() {
	motor_loop();
	motor_set_rpm(digitalRead(BUTTON_PIN) ? 0 : 25);
}
