#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <Arduino.h>
#include <Wire.h>
#include "globals.h"
#include "buffer.h"

#define DEGREES					(0xDF)
#define SEP						(' ')
#define FUL						(0xFF)
#define TOP						(1)
#define BOT						(2)
#define BACKSLASH				(3)
#define LEFT_DOT				(4)
#define RIGHT_DOT				(5)
//#define CELSIUS					(6)

#define LCD_DEFAULT_ADDR		(0x27)
#define LCD_CUSTOM_CHAR_SIZE	(8)
#define LCD_CUSTOM_CHAR_COUNT	(5)

#define LCD_ENTRY_MODE			(0x04)
#define LCD_DISPLAY_MODE		(0x08)
#define LCD_FUNCTION_MODE		(0x20)

#define LCD_SET_CGRAM_ADDR		(0x40)
#define LCD_SET_DDRAM_ADDR		(0x80)

#define LCD_ENTRY_LEFT			(0x02)
#define LCD_2_LINES				(0x08)

#define LCD_BLINK_ON			(0x01)
#define LCD_CURSOR_ON			(0x02)
#define LCD_DISPLAY_ON			(0x04)

#define LCD_BACKLIGHT_ON		(0x08)
#define LCD_BACKLIGHT_OFF		(0x00)

#define EXPANDER_REGISTER 		(0x01)
#define EXPANDER_READ 			(0x02)
#define EXPANDER_ENABLE			(0x04)


PCHARS(CUSTOM_CHARS_P,
	0x1f,0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,	// top
	0x00,0x00,0x00,0x00,0x00,0x1f,0x1f,0x1f,	// bot
	0x00,0x10,0x08,0x04,0x02,0x01,0x00,0x00,	// backslash
	0x00,0x01,0x03,0x07,0x07,0x03,0x01,0x00,	// left dot
	0x00,0x10,0x18,0x1c,0x1c,0x18,0x10,0x00,	// right dot
	//0x0e,0x11,0x11,0x11,0x0e,0x00,0x00,0x00,	// celsius
	//0x0e,0x1f,0x1f,0x1f,0x0e,0x00,0x00,0x00,	// celsius
);

class Screen : public Buffer {
	public:
		Screen();
		Screen(int16_t width, int16_t height, uint8_t address=LCD_DEFAULT_ADDR);
		void begin(int16_t width, int16_t height, uint8_t address=LCD_DEFAULT_ADDR);
		void drawBuffer(int16_t x, int16_t y, Buffer * buffer);
		void drawScreenArray(uint8_t * array);
		void setValue(int16_t x, int16_t y, uint8_t value);
		void rotate(int16_t x_offset, int16_t y_offset);
		void setBacklight(bool is_on);
		void setDisplay(bool is_on);
		void setCursorBlink(bool is_on);
		void setCursorVisibility(bool is_on);
		void setCursor(uint8_t x, uint8_t y);
		void drawArray();
	private:
		void init(int16_t width, int16_t height, uint8_t address);
		void drawChar(uint8_t c);
		void lcdUpdateDisplayMode();
		void lcdCommand(uint8_t data);
		void lcdWrite(uint8_t data);
		void lcdWriteNibble(uint8_t data);
		void expanderWrite(uint8_t data);
		uint8_t _address;
		uint8_t _display_mode;
		uint8_t _back_light_mask;
		uint8_t * _old_array;
};

#endif
