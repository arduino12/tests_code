#ifndef DOT_DISPLAY_H
#define DOT_DISPLAY_H

#include "font_5x7.h"

#define HCMS_DATA_PIN			(4)
#define HCMS_RS_PIN				(5)
#define HCMS_CLOCK_PIN			(6)
#define HCMS_ENABLE_PIN			(7)

#define DISPLAY_WIDTH			(80)
#define DISPLAY_HEIGHT			(7)

#define REGISTER_MODE_SERIAL	(0x80)
#define REGISTER_MODE_GLOBAL	(0x81)
#define REGISTER_BRIGHTNESS		(0x40)

#define DEFAULT_BRIGHTNESS		(10)


uint8_t dot_display_buffer[DISPLAY_WIDTH] = {0};

static uint8_t display_brightness = DEFAULT_BRIGHTNESS;

/* shift out one byte to the display */
static void send_data(uint8_t data) {
	uint8_t mask = 0x80;
	while (mask) {
		digitalWrite(HCMS_DATA_PIN, (data & mask) != 0);
		digitalWrite(HCMS_CLOCK_PIN, LOW);
		digitalWrite(HCMS_CLOCK_PIN, HIGH);
		mask >>= 1;
	}
	digitalWrite(HCMS_CLOCK_PIN, LOW);
}

/* send a register to the display */
static void send_register(uint8_t data) {
	digitalWrite(HCMS_RS_PIN, HIGH);		// select control register
	digitalWrite(HCMS_ENABLE_PIN, LOW);		// enable display writing
	send_data(data);						// shift data out
	digitalWrite(HCMS_ENABLE_PIN, HIGH);	// disable display writing
	digitalWrite(HCMS_RS_PIN, LOW);			// select dot register
}

/* set the brightness of the entier display */
void dot_display_set_brightness(uint8_t brightness) {
	if (brightness > 0x0F) brightness = 0x0F;
	display_brightness = brightness;
	send_register(REGISTER_MODE_GLOBAL);
	send_register(REGISTER_BRIGHTNESS | brightness);
	send_register(REGISTER_MODE_SERIAL);
}

/* flush the display_buffer to the display, also reconfigure it */
void dot_display_refresh() {
	dot_display_set_brightness(display_brightness);
	digitalWrite(HCMS_ENABLE_PIN, LOW);		// enable display writing
	for (uint8_t i = 0; i < DISPLAY_WIDTH; i++)
		send_data(dot_display_buffer[i]);	// shift data out
	digitalWrite(HCMS_ENABLE_PIN, HIGH);	// disable display writing
}

/* initialize the gpios and the hsmc display */
void dot_display_init() {
	pinMode(HCMS_DATA_PIN, OUTPUT);
	pinMode(HCMS_RS_PIN, OUTPUT);
	pinMode(HCMS_CLOCK_PIN, OUTPUT);
	pinMode(HCMS_ENABLE_PIN, OUTPUT);

	digitalWrite(HCMS_DATA_PIN, LOW);
	digitalWrite(HCMS_RS_PIN, LOW);
	digitalWrite(HCMS_CLOCK_PIN, LOW);
	digitalWrite(HCMS_ENABLE_PIN, HIGH);

	dot_display_refresh();
}

#endif /* DOT_DISPLAY_H */
