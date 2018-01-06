#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

#include "font_5x7.h"

/* define for single font display (width, height first char and char count defined in font file)*/
#define SINGLE_FONT FONT_5X7


class TextView : public View {
	public:
		TextView();

#ifdef SINGLE_FONT
		void begin(const char * text);
		void begin_P(PGM_P text_P);
#else

#endif
		void draw();
		
};

TextView tv;

tv.begin("hello");


/* print one char to the display_buffer */
void print_char(uint8_t c, uint8_t index) {
	if((c < FONT_FIRST_CHAR) || (c > (FONT_FIRST_CHAR + FONT_CHARS_COUNT))) return; // invalid char

	uint16_t font_index = (c - FONT_FIRST_CHAR) * FONT_WIDTH; // char table index
	for(uint8_t i = 0; i < FONT_WIDTH; i++) // each column
		dot_display_buffer[index + i] = FONT_READ_DATA(font_index + i);
}

/* print text to the display_buffer */
void print_text(char * text, uint8_t index) {
	int8_t length = min(DISPLAY_WIDTH, strlen(text) * FONT_WIDTH);
	while ((length > 0) && (index < DISPLAY_WIDTH)) {
		print_char(*text++, index);
		index += FONT_WIDTH;
		length -= FONT_WIDTH;
	}
}

void print_text_P(PGM_P text) {
	// strcpyP()
}


#endif /* TEXT_VIEW_H */
