#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <Arduino.h>
#include "globals.h"

class Buffer {
	public:
		Buffer();
		Buffer(Buffer * buffer);
		Buffer(int16_t width, int16_t height, const char * array=NULL);
		~Buffer();

		Buffer * begin(Buffer * buffer);
		Buffer * begin(int16_t width, int16_t height, const char * array=NULL);
		Buffer * begin_P(int16_t width, int16_t height, PGM_P array_P=NULL);
		void end();
		
		void resize(int16_t left, int16_t top, int16_t right, int16_t bottom);

		void clean();
		void fill(uint8_t value);
		void drawBuffer(int16_t x, int16_t y, Buffer * buffer);
		void rotate(int16_t x_offset, int16_t y_offset);
		void setValue(int16_t x, int16_t y, uint8_t value);

		uint8_t getValue(int16_t x, int16_t y);
		uint8_t * getArray();
		int16_t getWidth();
		int16_t getHeight();
		int16_t getIndex(int16_t x, int16_t y);
		int16_t getSize();

		bool isInRange(int16_t index);
		bool isInRange(int16_t x, int16_t y);
		bool isOverRange(int16_t x, int16_t y);
		bool isUnderRange(int16_t x, int16_t y);
	protected:
		uint8_t * _array = NULL;
		int16_t _width = 0, _height = 0;
	private:
		void init(Buffer * buffer);
		void init(int16_t width, int16_t height, const char * array);
		int16_t mod(int16_t a, int16_t b);
};

#endif
