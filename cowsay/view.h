#ifndef _VIEW_H_
#define _VIEW_H_

#include <Arduino.h>
#include "globals.h"
#include "screen.h"

#define DIGIT_WIDTH				(3)
#define DIGIT_HIGHT				(4)
#define DIGIT_COUNT				(10)

PCHARS(DIGITS_P,
	FUL, FUL, FUL, FUL, SEP, FUL, FUL, SEP, FUL, FUL, FUL, FUL,	// 0
	BOT, FUL, SEP, TOP, FUL, SEP, SEP, FUL, SEP, FUL, FUL, FUL,	// 1
	FUL, FUL, FUL, BOT, BOT, FUL, FUL, TOP, TOP, FUL, FUL, FUL,	// 2
	FUL, FUL, FUL, BOT, BOT, FUL, TOP, TOP, FUL, FUL, FUL, FUL,	// 3
	FUL, SEP, FUL, FUL, BOT, FUL, TOP, TOP, FUL, SEP, SEP, FUL,	// 4
	FUL, FUL, FUL, FUL, BOT, BOT, TOP, TOP, FUL, FUL, FUL, FUL,	// 5
	FUL, FUL, FUL, FUL, BOT, BOT, FUL, TOP, FUL, FUL, FUL, FUL,	// 6
	FUL, FUL, FUL, SEP, SEP, FUL, SEP, SEP, FUL, SEP, SEP, FUL,	// 7
	FUL, FUL, FUL, FUL, BOT, FUL, FUL, TOP, FUL, FUL, FUL, FUL,	// 8
	FUL, FUL, FUL, FUL, BOT, FUL, TOP, TOP, FUL, FUL, FUL, FUL,	// 9
);

class View : public Buffer {
	public:
		View();
		View(Buffer * buffer);
		View(int16_t width, int16_t height, const char * array=NULL);
		
		void clear();

		Buffer * beginDigit(uint8_t digit);
		Buffer * beginText(const char * text, char empty_char=' ');
		Buffer * beginText_P(PGM_P text_P, char empty_char=' ');
		
		void drawBufferOnCenter(Buffer * buffer);
		int16_t getTextWidth(const char * text);
		int16_t getTextHeight(const char * text);
		int16_t getTextWidth_P(PGM_P text_P);
		int16_t getTextHeight_P(PGM_P text_P);
	private:
		int16_t indexOf(const char * text, char value);
};

#endif
