#include "view.h"

View::View() : Buffer() { }
View::View(Buffer * buffer) : Buffer(buffer) { }
View::View(int16_t width, int16_t height, const char * array) : Buffer(width, height, array) { }

void View::clear() {
	fill('\0');
}

Buffer * View::beginDigit(uint8_t digit) {
	return Buffer::begin_P(DIGIT_WIDTH, DIGIT_HIGHT, &DIGITS_P[((digit) % DIGIT_COUNT) * DIGIT_WIDTH * DIGIT_HIGHT]);
}

Buffer * View::beginText(const char * text, char empty_char) {
	int16_t width = getTextWidth(text), height = getTextHeight(text);
	Buffer * buffer = Buffer::begin(width, height);
	if (NULL == buffer) return NULL;
	uint8_t * ptr = _array;

	for (int16_t index = 0, sep; -1 != index; text += (index + 1)) {
		index = indexOf(text, '\n');
		sep = (-1 != index ? index : strlen(text));
		memcpy(ptr, text, sep);
		memset(ptr + sep, empty_char, _width - sep);
		ptr += _width;
	}

	return buffer;
}

Buffer * View::beginText_P(PGM_P text_P, char empty_char) {
	char text[strlen_P(text_P) + 1];
	strcpy_P(text, text_P);
	return beginText(text, empty_char);
}

void View::drawBufferOnCenter(Buffer * buffer) {
	//drawBuffer((_width - buffer->_width) / 2, (_height - buffer->_height) / 2, buffer);
	drawBuffer((_width - buffer->getWidth()) / 2, (_height - buffer->getHeight()) / 2, buffer);
	//Buffer::drawBuffer(0,0, buffer);
}

int16_t View::getTextWidth(const char * text) {
	int16_t return_value = 0;
	
	for (int16_t index = 0;; text += (index + 1)) {
		index = indexOf(text, '\n');
		if (-1 == index) {
			int16_t len = strlen(text);
			return (len > return_value ? len : return_value);
		}
		if (return_value < index) return_value = index;
	}

	return -1;
}

int16_t View::getTextHeight(const char * text) {
	int16_t return_value = 0;
	
	for (int16_t index = 0; -1 != index; return_value++, text += (index + 1))
		index = indexOf(text, '\n');

	return return_value;
}

int16_t View::indexOf(const char * text, char value) {
	int16_t return_value = -1;
	char c;
	
	while (c = text[++return_value])
		if (c == value)
			return return_value;
	return -1;
}

int16_t View::getTextWidth_P(PGM_P text_P) {
	char text[strlen_P(text_P) + 1];
	strcpy_P(text, text_P);
	return getTextWidth(text);
}

int16_t View::getTextHeight_P(PGM_P text_P) {
	char text[strlen_P(text_P) + 1];
	strcpy_P(text, text_P);
	return getTextHeight(text);
}
