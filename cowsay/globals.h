#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>

#define DEBUG					// debug to uart (serial) if defined
#ifdef DEBUG
	#define DEBUG_INIT			Serial.begin(115200)
	#define DEBUG_WRITE(...)	Serial.write(__VA_ARGS__)
	#define DEBUG_PRINT(...)	Serial.print(__VA_ARGS__)
	#define DEBUG_PRINTLN(...)	Serial.println(__VA_ARGS__)
#else
	#define DEBUG_INIT
	#define DEBUG_WRITE(...)
	#define DEBUG_PRINT(...)
	#define DEBUG_PRINTLN(...)
#endif

#define TOUCH_PIN				(A3)
#define SDA_PIN					(A4)
#define SCL_PIN					(A5)
#define JOY_Y_PIN				(A6)
#define JOY_X_PIN				(A7)

#define LCD_WIDTH				(20)
#define LCD_HIGHT				(4)

#define MIN_PER_HOUR			(60)
#define MIN_PER_DAY				(SECS_PER_DAY / SECS_PER_MIN)

#define DEFAULTE_TIMER			(10)
#define CLOCK_DOT_MS			(650)

#define pgm_read_ptr(address)	(void *)__LPM_word((uint16_t)(address))
#define PARRAY(name, ...)		const name[] PROGMEM = {__VA_ARGS__}
#define PCHARS(name, ...)		char PARRAY(name, __VA_ARGS__)

#endif
