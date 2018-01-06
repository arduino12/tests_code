#include <Time.h>
#include <Wire.h>
#include <DS3232RTC.h>
#include <Buttons.h>

#include "globals.h"
#include "texts.h"
#include "buffer.h"
#include "view.h"
#include "screen.h"
#include "standby.h"

View view;
View screen_view;
	
Screen screen;
Joystick joystick;

uint8_t (*modeInit)();
uint8_t (*modeLoop)();
		
uint16_t temperature, old_temperature = 0;
uint16_t minutes, old_minutes = 0;

char * joystick_directions[] = {
	"MIDDLE", "LEFT", "LEFT UP", "UP", "RIGHT UP",
	"RIGHT", "RIGHT DOWN", "DOWN", "LEFT DOWN",
};

void setup() {
	DEBUG_INIT;
	
//	setTime(20, 0, 0, 2, 5, 2016);
	setSyncProvider(RTC.get);

	screen.begin(LCD_WIDTH, LCD_HIGHT);
	screen.setBacklight(true);
	
	joystick.begin(
		JOY_X_PIN,
		JOY_Y_PIN,
		TOUCH_PIN,
		400,
		700,
		400,
		700
	);
	joystick.setPressMillis(
		10,
		500,
		100
	);
	joystick.setShortPressFunc(
		onShortPressJoystick
	);
	/*
	joystick.setShortReleaseFunc(
		onShortPressJoystick
	);*/
	pinMode(TOUCH_PIN, INPUT);
	
	DEBUG_PRINTLN(F("start!"));
	//modeInit = &standbyBegin;
	//modeLoop = &standbyLoop;
	//modeInit();
	screen_view.begin(&screen);
}

void loop() {
	//static View view;
	//static View screen_view(&screen);
	
	joystick.update();
	//screen.rotate(-1, 0);
	//drawTime(getCurTime());
/*
	temperature = RTC.getTemperature();
	if (old_temperature != temperature) {
		old_temperature = temperature;
		screen_view.clean();
		screen_view.drawBufferOnCenter(getTemperatureView(&view, temperature));
		//screen.drawBuffer(0, 0, getTemperatureView(&view, temperature));
	}
*/

	minutes = getMinutesTime();
	if (old_minutes != minutes) {
		old_minutes = minutes;
		getMinutesTimeView(&view, minutes);
		//screen.drawBuffer(0, 0, getMinutesTimeView(&view, minutes));
	}
	screen_view.clean();
	screen_view.drawBufferOnCenter(blinkMinutesTimeViewDots(&view));
	//screen.drawBuffer(0, 0, blinkMinutesTimeViewDots(&view));
	
	//screen.drawScreenArray(screen_view.getArray());


	//modeLoop();
	//DEBUG_PRINTLN(analogRead(A6));

	screen.drawScreenArray(screen_view.getArray());
	delay(500);
}

View * getTemperatureView(View * view, uint16_t temperature) {
	int16_t x = 0;
	view->begin(18, 4);
	x = printDigit(x, 0, view, (temperature & 0xFF) / 10);
	x = printDigit(x, 0, view, (temperature & 0xFF) % 10);
	x = printText_P(x, 3, view, DOT_P);
	x = printDigit(x, 0, view, temperature >> 8);
	x = printText_P(x, 0, view, CELSIUS_P) - 1;
	view->resize(0, 0, x - view->getWidth(), 0);
	return view;
}

View * getMinutesTimeView(View * view, uint16_t minutes) {
	uint8_t hours = minutes / MIN_PER_HOUR;
	int16_t x = 0;
	minutes %= MIN_PER_HOUR;
	view->begin(18, 4);
	x = printDigit(x, 0, view, hours / 10);
	x = printDigit(x, 0, view, hours % 10);
	x = printDigit(x + 3, 0, view, minutes / 10);
	x = printDigit(x, 0, view, minutes % 10) - 1;
	view->resize(0, 0, x - view->getWidth(), 0);
	return view;
}

View * blinkMinutesTimeViewDots(View * view) {
	static bool is_dots = false;
	printText_P(view->getWidth() / 2 - 1, 1, view, (is_dots ? DOTS_P : PSTR("  \n  ")));
	is_dots = !is_dots;
	return view;
}

int16_t printText_P(int16_t x, int16_t y, View * view, PGM_P text_P) {
	View converter;
	view->drawBuffer(x, y, converter.beginText_P(text_P));
	return x + converter.getWidth() + 1;
}

int16_t printText(int16_t x, int16_t y, View * view, char * text) {
	View converter;
	view->drawBuffer(x, y, converter.beginText(text));
	return x + converter.getWidth() + 1;
}

int16_t printDigit(int16_t x, int16_t y, View * view, uint8_t digit) {
	View converter;
	view->drawBuffer(x, y, converter.beginDigit(digit));
	return x + DIGIT_WIDTH + 1;
}

uint16_t getMinutesTime() {
	return (now() % SECS_PER_DAY) / SECS_PER_MIN;
}

void onShortPressJoystick() {
	screen_view.clean();
	view.beginText(joystick_directions[joystick.getDirection()]);
	screen_view.drawBuffer(0, 0, &view);
	/*
	DEBUG_PRINT(F("joystick direction:\n"));
	DEBUG_PRINT(joystick_directions[joystick.getDirection()]);
	DEBUG_PRINT(F("\n\n"));*/
}