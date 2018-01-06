//{ includes:
#include "globals.h"
#include "led_strip.h"
#include <Buttons.h>
//}

//{ defines:
//}

//{ globals
Keypad keypad_3x3;
//}

//{ helpers


void onShortPress3x4() {
	Serial.print(F("press begin:\n"));
	Serial.print(F("buttons = 0x"));
	Serial.print(keypad_3x3.getButtonsBitmask(), HEX);
	Serial.print(F(", key = "));
	Serial.print(keypad_3x3.getButtonIndex());
	Serial.print(F(", millis up time = "));
	Serial.print(keypad_3x3.getUpMillis(), DEC);
	Serial.print(F("\n"));
	led_strip_set_color(keypad_3x3.getButtonIndex()-1, COLOR_MAGENTA);
}

void onShortRelease3x4() {
	Serial.print(F("short press completed:\n"));
	Serial.print(F("millis = "));
	Serial.print(keypad_3x3.getDownMillis(), DEC);
	Serial.print(F("\n\n"));
	led_strip_set_color(keypad_3x3.getButtonIndex()-1, COLOR_BLACK);
}

void onLongPress3x4() {
	Serial.print(F("long press begin:\n"));
	Serial.print(F("millis = "));
	Serial.print(keypad_3x3.getDownMillis(), DEC);
	Serial.print(F("\n"));
}

void onLongRelease3x4() {
	Serial.print(F("\nlong press completed:\n"));
	Serial.print(F("millis = "));
	Serial.print(keypad_3x3.getDownMillis(), DEC);
	Serial.print(F(", pulses = "));
	Serial.print(keypad_3x3.getPulseCount(), DEC);
	Serial.print(F("\n\n"));
}

void onLongPulse3x4() {
	uint32_t down_millis = keypad_3x3.getDownMillis();
	
	if (down_millis < 4000)
		keypad_3x3.setNextPulseMillis(1000);
	else if (down_millis < 7000)
		keypad_3x3.setNextPulseMillis(400);
	else
		keypad_3x3.setNextPulseMillis(100);
	
	Serial.print('.');
}


//}

//{ setup and loop
void setup() {
	/* initialize the debug UART */
	DEBUG_INIT;
	DEBUG_PRINTLN(F("setup"));
	/* initialize the RGB LED strip */
	led_strip_init();

	led_strip_set_color(0, COLOR_RED);
	led_strip_set_color(1, COLOR_GREEN);
	led_strip_set_color(2, COLOR_BLUE);
	led_strip_set_color(3, COLOR_BLACK);
	led_strip_set_color(4, COLOR_BLACK);
	led_strip_set_color(5, COLOR_BLACK);
	led_strip_set_color(6, COLOR_BLACK);
	led_strip_set_color(7, COLOR_BLACK);
	led_strip_set_color(8, COLOR_BLACK);
	led_strip_set_color(9, COLOR_BLACK);

	uint8_t row_pins[]		= {A0, A1, A2};
	uint8_t column_pins[]	= {8, 9, 10};
	
	keypad_3x3.begin(
		row_pins,					// row pins
		column_pins,				// column pins
		sizeof(row_pins),			// rows count
		sizeof(column_pins)			// columns count
	);
	keypad_3x3.setPressMillis(
		20,							// short_press_millis - the minimal amount of milliseconds for a "short press".
		1000,						// long_press_millis - the minimal amount of milliseconds for a "long press".
		800							// pulse_press_millis - the amount of milliseconds between "long pulse".
	);
	keypad_3x3.setEventsFuncs(
		onShortPress3x4,			// function that will be called once when button is down for more then short_press_millis.
		onShortRelease3x4,			// function that will be called once the button released after less then long_press_millis.
		onLongPress3x4,				// function that will be called once when button is down for more then long_press_millis.
		onLongRelease3x4,			// function that will be called once the button released after more then long_press_millis.
		onLongPulse3x4				// function that will be called once when button is down and next_pulse_millis passed.
	);
	
	/* start infinity loop */
	DEBUG_PRINTLN(F("loop"));
}

void loop() {
	static uint8_t i = 0;
	
	/* update the keypad states and call events if needed */
	keypad_3x3.update();
	
	led_strip_refresh();
	delay(1);
}
//}
