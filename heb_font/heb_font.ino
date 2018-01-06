#include "utf8.h"

#define SERIAL_BAUD_RATE	(115200)
#define SERIAL_TIMEOUT		(50)

const uint8_t HEBREW_TEST[] = "אבגדהו זחט";
const uint8_t HEB_FIRST_CHAR[] = "א";
/*
struct font_t {
	uint8_t height;
	uint8_t sequences_count;
		uint16_t unicode_start;
		uint8_t characters_count;
		uint16_t tabel_start;
};
*/


void graphics_draw_unicode_char(unicode_t ch) {
	Serial.print("0x");
	Serial.println(ch, HEX);
}

void graphics_draw_unicode_text(unicode_t * text) {
	unicode_t ch;
	while (ch = *text++)
		graphics_draw_unicode_char(ch);
}
/*

void graphics_draw_utf8_text(const uint8_t * text) {
	uint16_t unicode_text_len = strlen(text) + 1;
	unicode_t * unicode_text = malloc(unicode_text_len * sizeof(unicode_t));
	utf8_to_unicode(text, unicode_text, unicode_text_len);
	graphics_draw_unicode_text(unicode_text);
	free(unicode_text);
}

void graphics_draw_unicode_text(uint8_t * text, bool rtl) {
	uint8_t stl = strlen(text);
	if (stl == 0) return;
}

void graphics_draw_utf8_text(uint8_t * text) {
	uint8_t c;
	uint32_t unicode;
	while (c = *text++) {
		if (c < 0x80)
			graphics_draw_unicode_char(c);
		else {
			
			while (c & 0x80) {
				
			}
		}
	}
		Serial.println(*text++, HEX);
}
*/
void setup() {
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.setTimeout(SERIAL_TIMEOUT);
	Serial.println(F("start!"));
	// Serial.println(sizeof(HEB_MAZAL_TOV_2));
	// graphics_draw_utf8_text(HEBREW_TEST);
	unicode_t dst[60];
	utf8_to_unicode(HEBREW_TEST, dst, 60);
	graphics_draw_unicode_text(dst);
	Serial.println(F("loop!"));
}

void loop() {
}
