#include "buffer.h"

Buffer::Buffer() {
	_width = 0;
	_height = 0;
	_array = NULL;
}

Buffer::Buffer(Buffer * buffer) {
	begin(buffer);
}

Buffer::Buffer(int16_t width, int16_t height, const char * array) {
	begin(width, height, array);
}

Buffer::~Buffer() {
	end();
}

Buffer * Buffer::begin(Buffer * buffer) {
	init(buffer);
	return this;
}

Buffer * Buffer::begin(int16_t width, int16_t height, const char * array) {
	init(width, height, array);
	if (NULL == array) clean();
	return this;
}

Buffer * Buffer::begin_P(int16_t width, int16_t height, PGM_P array_P) {
	char array[width * height];
	memcpy_P(array, array_P, width * height);
	return begin(width, height, array);
}

void Buffer::end() {
	if (NULL == _array) return;
	free(_array);
	_array = NULL;
	_width = 0;
	_height = 0;
}

void Buffer::init(Buffer * buffer) {
	init(buffer->_width, buffer->_height, (char *)buffer->_array);
}

void Buffer::init(int16_t width, int16_t height, const char * array) {
	end();
	if (isUnderRange(width, height)) return;
	_width = width;
	_height = height;
	_array = (uint8_t *)malloc(getSize());
	if (NULL == _array) {
		while (1);
	}
	if (NULL != array) memcpy(_array, array, getSize());
}

void Buffer::resize(int16_t left, int16_t top, int16_t right, int16_t bottom) {
	Buffer buffer(this);
	end();
	begin(buffer._width + left + right, buffer._height + top + bottom);
	drawBuffer(left, top, &buffer);
}

void Buffer::clean() {
	fill(' ');
}

void Buffer::fill(uint8_t value) {
	memset(_array, value, getSize());
}

void Buffer::drawBuffer(int16_t x, int16_t y, Buffer * buffer) {
	if ((NULL == buffer) || isOverRange(x, y)) return;

	uint8_t start_x, start_y, end_x, end_y;
	
	if (x >= 0) start_x = x;
	else if ((x + buffer->_width) >= 0) start_x = 0;
	else return;
	
	if (y >= 0) start_y = y;
	else if ((y + buffer->_height) >= 0) start_y = 0;
	else return;
	
	end_x = x + buffer->_width;
	end_y = y + buffer->_height;
	
	if (end_x > _width) end_x = _width;
	if (end_y > _height) end_y = _height;
	
	for (uint8_t y_index = start_y; y_index < end_y; y_index++)
		for (uint8_t x_index = start_x; x_index < end_x; x_index++)
			setValue(x_index, y_index, buffer->getValue(x_index - x, y_index - y));
}

void Buffer::rotate(int16_t x_offset, int16_t y_offset) {
	Buffer buffer(this);
	for (uint8_t y = 0; y < _height; y++)
		for (uint8_t x = 0; x < _width; x++)
			setValue(mod(x + x_offset,_width), mod(y + y_offset, _height), buffer.getValue(x, y));
}

int16_t Buffer::mod(int16_t a, int16_t b) {
	return (a < 0 ? b - ((-a) % b) : a % b);
}

void Buffer::setValue(int16_t x, int16_t y, uint8_t value) {
	if (isInRange(x, y))
		*(_array + getIndex(x, y)) = value;
}

uint8_t Buffer::getValue(int16_t x, int16_t y) {
	if (isInRange(x, y))
		return *(_array + getIndex(x, y));
	return 0;
}

uint8_t * Buffer::getArray() {
	return _array;
}

int16_t Buffer::getWidth() {
	return _width;
}

int16_t Buffer::getHeight() {
	return _height;
}

int16_t Buffer::getIndex(int16_t x, int16_t y) {
	return x + y * _width;
}

int16_t Buffer::getSize() {
	return _width * _height;
}

bool Buffer::isInRange(int16_t index) {
	return (index >= 0) && (index < getSize());
}

bool Buffer::isInRange(int16_t x, int16_t y) {
	return (x >= 0) && (y >= 0) && (x < _width) && (y < _height);
}

bool Buffer::isOverRange(int16_t x, int16_t y) {
	return (x >= _width) || (y >= _height);
}

bool Buffer::isUnderRange(int16_t x, int16_t y) {
	return (x < 0) || (y < 0);
}