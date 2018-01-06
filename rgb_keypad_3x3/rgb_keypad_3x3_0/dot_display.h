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

static void send_register(uint8_t data) {
	digitalWrite(HCMS_RS_PIN, HIGH);		// select control register
	digitalWrite(HCMS_ENABLE_PIN, LOW);		// enable display writing
	send_data(data);						// shift data out
	digitalWrite(HCMS_ENABLE_PIN, HIGH);	// disable display writing
	digitalWrite(HCMS_RS_PIN, LOW);			// select dot register
}

void dot_display_set_brightness(uint8_t brightness) {
	if (brightness > 0x0F) brightness = 0x0F;
	display_brightness = brightness;
	send_register(REGISTER_MODE_GLOBAL);
	send_register(REGISTER_BRIGHTNESS | brightness);
	send_register(REGISTER_MODE_SERIAL);
}

void dot_display_refresh() {
	dot_display_set_brightness(display_brightness);
	digitalWrite(HCMS_ENABLE_PIN, LOW);		// enable display writing
	for (uint8_t i = 0; i < DISPLAY_WIDTH; i++)
		send_data(dot_display_buffer[i]);	// shift data out
	digitalWrite(HCMS_ENABLE_PIN, HIGH);	// disable display writing
}

/* initialize hsmc display */
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

void print_char(uint8_t c, uint8_t index) {
	if((c < FONT_FIRST_CHAR) || (c > (FONT_FIRST_CHAR + FONT_CHARS_COUNT))) return; // invalid char

	uint16_t font_index = (c - FONT_FIRST_CHAR) * FONT_WIDTH; // char table index
	for(uint8_t i = 0; i < FONT_WIDTH; i++) // each column
		dot_display_buffer[index + i] = FONT_READ_DATA(font_index + i);
}

void print_text(char * text, uint8_t index) {
	uint8_t length = min(DISPLAY_WIDTH, strlen(text) * FONT_WIDTH);
	while (index < length) {
		print_char(*text++, index);
		index += FONT_WIDTH;
	}
}

#endif /* DOT_DISPLAY_H */
