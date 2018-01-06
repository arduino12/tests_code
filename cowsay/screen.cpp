#include "screen.h"

Screen::Screen() : Buffer() { 
	_old_array = NULL;
}

Screen::Screen(int16_t width, int16_t height, uint8_t address) : Buffer(width, height) {
	init(width, height, address);
}

void Screen::begin(int16_t width, int16_t height, uint8_t address) {
	Buffer::begin(width, height);
	init(width, height, address);
}

void Screen::init(int16_t width, int16_t height, uint8_t address) {
	PGM_P custom_chars_ptr = CUSTOM_CHARS_P;
	
	_address = address;
	_display_mode = 0;
	_back_light_mask = LCD_BACKLIGHT_OFF;
	_old_array = (uint8_t *)malloc(getSize());
	memset(_old_array, '\0', getSize());
	
	Wire.begin();
	lcdWriteNibble(0x30);
	delayMicroseconds(4500);
	lcdWriteNibble(0x30);
	delayMicroseconds(4500);
	lcdWriteNibble(0x30);
	delayMicroseconds(150);
	lcdWriteNibble(0x20);
	
	lcdCommand(LCD_FUNCTION_MODE | LCD_2_LINES);
	lcdCommand(LCD_ENTRY_MODE | LCD_ENTRY_LEFT);
	
	for (uint8_t i = 0; i < LCD_CUSTOM_CHAR_COUNT; i++) {
		lcdCommand(LCD_SET_CGRAM_ADDR | (i << 3));
		for (uint8_t j = 0; j < LCD_CUSTOM_CHAR_SIZE; j++)
			lcdWrite(pgm_read_byte(custom_chars_ptr++));	
	}

	drawArray();
	setDisplay(true);
}

void Screen::drawBuffer(int16_t x, int16_t y, Buffer * buffer) {
	Buffer::drawBuffer(x, y, buffer);
	drawArray();
}

void Screen::setValue(int16_t x, int16_t y, uint8_t value) {
	Buffer::setValue(x, y, value);
	drawArray();
}

void Screen::rotate(int16_t x_offset, int16_t y_offset) {
	Buffer::rotate(x_offset, y_offset);
	drawArray();
}

void Screen::drawScreenArray(uint8_t * array) {
	memcpy(_array, array, getSize());
	drawArray();
}

void Screen::drawArray() {
	uint8_t * array = _array;
	uint8_t * old_array = _old_array;
	bool old_char_didnt_chaneged = true;
	for (uint8_t y = 0; y < _height; y++)
		for (uint8_t x = 0; x < _width; x++) {
			if (*old_array != *array) {
				*old_array = *array;
				if (old_char_didnt_chaneged) setCursor(x, y);
				drawChar(*array);
				old_char_didnt_chaneged = false;
			}
			else old_char_didnt_chaneged = true;
			array++;
			old_array++;
		}
}

void Screen::drawChar(uint8_t c) {
	if ('\\' == c) lcdWrite(BACKSLASH - 1);
	else if (c > LCD_CUSTOM_CHAR_COUNT) lcdWrite(c);
	else if ('\0' != c) lcdWrite(c - 1);
}

void Screen::setBacklight(bool is_on) {
	_back_light_mask = (is_on ? LCD_BACKLIGHT_ON : LCD_BACKLIGHT_OFF);
	expanderWrite(0);
}

void Screen::setDisplay(bool is_on) {
	if (is_on)	_display_mode |= LCD_DISPLAY_ON;
	else		_display_mode &= ~LCD_DISPLAY_ON;
	lcdUpdateDisplayMode();
}

void Screen::setCursorBlink(bool is_on) {
	if (is_on)	_display_mode |= LCD_BLINK_ON;
	else		_display_mode &= ~LCD_BLINK_ON;
	lcdUpdateDisplayMode();
}

void Screen::setCursorVisibility(bool is_on) {
	if (is_on)	_display_mode |= LCD_CURSOR_ON;
	else		_display_mode &= ~LCD_CURSOR_ON;
	lcdUpdateDisplayMode();
}

void Screen::setCursor(uint8_t x, uint8_t y) {
	if (isOverRange(x, y)) return;
	if (y & 0x01) x += 0x40;
	if (y & 0x02) x += 0x14;
	lcdCommand(LCD_SET_DDRAM_ADDR | x);
}

void Screen::lcdUpdateDisplayMode() {
	lcdCommand(LCD_DISPLAY_MODE | _display_mode);
}

void Screen::lcdCommand(uint8_t data) {
	lcdWriteNibble(data & 0xF0);
	lcdWriteNibble(data << 0x4);
}

void Screen::lcdWrite(uint8_t data) {
	lcdWriteNibble(data & 0xF0 | EXPANDER_REGISTER);
	lcdWriteNibble(data << 0x4 | EXPANDER_REGISTER);
}

void Screen::lcdWriteNibble(uint8_t data) {
	expanderWrite(data | EXPANDER_ENABLE);
	delayMicroseconds(1);
	expanderWrite(data & ~EXPANDER_ENABLE);
	delayMicroseconds(50);
}

void Screen::expanderWrite(uint8_t data) {
	Wire.beginTransmission(_address);
	Wire.write(data | _back_light_mask);
	Wire.endTransmission();
}
