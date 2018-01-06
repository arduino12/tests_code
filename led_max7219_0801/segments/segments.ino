/*

MAX7219 ON 8 * 7 Segment Screen:

/=====================\
|   8 8 8 8 8 8 8 8   |
\=====================/
      | | | | | |
      1 2 3 4 5 6
	  
Pinout:
	1 = VCC (+5V)
	2 = GND
	3 = CLK
	4 = DATA
	5 = LOAD
	6 = DOUT
*/
#include "segments_font.h"

#define MAX7219_CLK_PIN_MASK	(0x01)
#define MAX7219_DATA_PIN_MASK	(0x02)
#define MAX7219_LOAD_PIN_MASK	(0x04)

#define MAX7219_DIGITS_COUNT	(8)
#define MAX7219_SEGMENTS_COUNT	(8)
#define BUFF_SIZE				(8)

#define pgm_read_ptr(address)	(void *)__LPM_word((uint16_t)(address))
#define PARRAY(name, ...)		const name[] PROGMEM = {__VA_ARGS__}
#define PCHARS(name, ...)		char PARRAY(name, __VA_ARGS__)


/*
PCHARS(TEXT_00_P, "arad is the best");
PCHARS(TEXT_01_P, "chef dallia presents");
PCHARS(TEXT_02_P, "love you");
PCHARS(TEXT_03_P, "fast-food");
PCHARS(TEXT_04_P, "good-job");
PCHARS(TEXT_05_P, "bon-appetit");
PCHARS(TEXT_06_P, "enjoy eating");
PCHARS(TEXT_07_P, "not cold");
PCHARS(TEXT_08_P, "redy and done");
PCHARS(TEXT_09_P, "be careful -hot-");
PCHARS(TEXT_10_P, "delicious taste");
PCHARS(TEXT_11_P, "be hungry");
PCHARS(TEXT_12_P, "-arduino-");
PCHARS(TEXT_13_P, "^^a-e-tec_2015^^");
PCHARS(TEXT_14_P, "_-^-_-^-_-^-_-^-_-^-_-^-_");

// char * PARRAY(TEXTS_ARRAY_PP,
PGM_P PARRAY(TEXTS_ARRAY_PP,
	TEXT_00_P, TEXT_01_P, TEXT_02_P, TEXT_03_P, TEXT_04_P,
	TEXT_05_P, TEXT_09_P, TEXT_07_P, TEXT_08_P, TEXT_09_P,
	TEXT_10_P, TEXT_11_P, TEXT_12_P, TEXT_13_P, TEXT_14_P
);
*/

//}

void max7219_init();
void max7219_send(uint8_t addres, uint8_t value);
void max7219_pulse(uint8_t mask);
void max7219_change(bool is_high, uint8_t mask);

void sendSegment(uint8_t segment, int8_t index);
void sendSegments(uint8_t * segments, int8_t index = 0, uint8_t count = 8);
void clearSegment(int8_t index);
void clearSegments(int8_t index = 0, uint8_t count = 8);

void sendChar(char character, int8_t index, bool with_dot = false);
void sendChars(char * characters, int8_t index = 0, uint8_t dots_mask = 0);
void sendChars_P(const char * characters, int8_t index = 0, uint8_t dots_mask = 0);

void itoa2(char * buffer, int32_t number, uint8_t digits = MAX7219_DIGITS_COUNT, bool leading_zeros = false);
void printIntegr(int32_t number, int8_t index = 0, uint8_t digits = MAX7219_DIGITS_COUNT, bool leading_zeros = false);
void printDouble(double number, int8_t index = 0, uint8_t left_digits = MAX7219_DIGITS_COUNT,
				 uint8_t right_digits = MAX7219_DIGITS_COUNT, bool left_zeros = false, bool right_zeros = false);


void setup() {
	Serial.begin(115200);				// initialize serial to 115200 baud
	Serial.println(F("setup()"));		// use serial for debug printing
	
	max7219_init();
	clearSegments();				// turn off all digits and dots
	
	sendChars("3.14");			// bad because full digit shows only '.'
	delay(1000);
	clearSegments(0, 4);			// clear only the used digits
	printDouble(3.14);			// solution..!
	delay(1000);
	
	char buffer[5];						// 1234 = 4 digits + null = 5
	itoa2(buffer, 1234);			// print the number to the buffer
	for (int8_t i = MAX7219_DIGITS_COUNT; i > -MAX7219_DIGITS_COUNT; i--) {
		sendChars(buffer, i);	// send the buffer to display digits
		delay(300);
		clearSegments(i, 4);		// clear only the used digits
	}
	
	printIntegr(28);				// 28
	sendSegment(0x63, 2);		// 28o
	sendChar('C', 3);			// 28oC
	delay(1500);
	
	sendChars("bad...");			// string saved in DRAM
	delay(1500);
	sendChars_P(PSTR("good !"));	// string is stored in FLASH
	delay(1500);
	
	for (int i = 1; i < 9; i++)
     	for (int j = 2; j < 511; j <<= 1)
		{
			max7219_send(i, j-1);
			delay(30);
		}
	for (int i = 1; i < 9; i++)
     	for (int j = 511; j > 0; j >>= 1)
		{
			max7219_send(i, j-1);
			delay(30);
		}
	
	Serial.println(F("loop()"));		// going to endless loop
}

void loop() {
	char buffer[] = "ArAd";						// 1234 = 4 digits + null = 5
	for (int8_t i = MAX7219_DIGITS_COUNT; i > -MAX7219_DIGITS_COUNT; i--) {
		sendChars(buffer, i);	// send the buffer to display digits
		delay(300);
		clearSegments(i, 4);		// clear only the used digits
	}
}



/*
	digitalWrite(CLK_PIN,	LOW);
	digitalWrite(LOAD_PIN,	LOW);
	digitalWrite(DATA_PIN,	LOW);
	SentData(11, 7);    // Max Scan Limit
	SentData(9, 0);     // Matrix Decode Mode
	SetMatrixIntensity(15);   // Medium Intensity (1-15)

	for (int i = 1; i < 9; i++)
		SentData(i, 0); //All Red And Green Leds = Off

	SetMatrixPower(true);     //Shutdown Mode
	
void SetMatrixPower(bool value)
{
	SentData(12, value ? 1 : 0);
}

void SetMatrixIntensity(int value)
{
	SentData(10, value);
}
*/
/*
void setBrightness(uint8_t brightness) {
	if (brightness > MAX_BRIGHTNESS) brightness = MAX_BRIGHTNESS;
	if (brightness)	sendCommand(0x88 | brightness);
	else			sendCommand(0x80);
}
*/

void max7219_init()
{
	DDRC  |= 0x07;				// (A0 - A2)	OUTPUT
	PORTC &= 0xF8;				// (A0 - A2)	LOW
	max7219_send(9, 0);			// set matrix Decode Mode
	max7219_send(10, 15);		// set max intensity (1-15)
	max7219_send(11, 7);		// set max scan limit
	max7219_send(12, 1);		// cancel shutdown Mode
	for (int i = 0; i < MAX7219_DIGITS_COUNT; i++)
		max7219_send(i + 1, 0);		// clear segments
}

void sendSegment(uint8_t segment, int8_t index) {
	if ((index >= 0) && (index < MAX7219_DIGITS_COUNT))
		max7219_send(index + 1, segment);
}

void sendSegments(uint8_t * segments, int8_t index, uint8_t count) {
	while (count--)
		sendSegment(*segments++, index++);
}

void clearSegment(int8_t index) {
	sendSegment(0, index);
}

void clearSegments(int8_t index, uint8_t count) {
	for (uint8_t i = 0; i < MAX7219_DIGITS_COUNT; i ++)
		max7219_send(i + 1, 0);
}

void sendChar(char character, int8_t index, bool with_dot) {
	if ((character >= 32) && (character < 127))
		sendSegment(pgm_read_byte(SEGMENTS_FONT + character - 32) | (with_dot ? 0x80 : 0), index);
}

void sendChars(char * characters, int8_t index, uint8_t dots_mask) {
	while (uint8_t c = *characters++)
		sendChar(c, index++, ((dots_mask >> index) & 1));
}

void sendChars_P(const char * characters, int8_t index, uint8_t dots_mask) {
	while (uint8_t c = pgm_read_byte(characters++))
		sendChar(c, index++, (dots_mask >> index) & 1);
}

void printDouble(double number, int8_t index, uint8_t left_digits, uint8_t right_digits, bool left_zeros, bool right_zeros) {
	char buffer[left_digits + right_digits + 1];
	itoa2(buffer, number, left_digits, left_zeros);
	uint8_t left_len = strlen(buffer);
	
	if (number < 0) number = -number;
	number -= (uint32_t)number;
	//number += 0.0001;
	left_digits = 0;
	while ((number - (uint32_t)number) > (0.06 * left_digits)) {
		number *= 10;
		left_digits++;
	}
	
	if (number) {
		if (right_digits > left_digits) right_digits = left_digits;
		itoa2(buffer + left_len, number, right_digits, true);
	}
	else {
		if (right_zeros)	memset(buffer + left_len, '0', right_digits);
		else				left_len = 9;
	}
	
	sendChars(buffer, index, 1 << (left_len - 1));
}

void printIntegr(int32_t number, int8_t index, uint8_t digits, bool leading_zeros) {
	char buffer[digits + 1];
	itoa2(buffer, number, digits, leading_zeros);
	sendChars(buffer, index);
}

void itoa2(char * buffer, int32_t number, uint8_t digits, bool leading_zeros) {
	if (0 == digits || NULL == buffer) return;
	if (number < 0) {
		*buffer++ = '-';
		number = -number;
		digits--;
	}
	uint32_t number2 = number;
	uint32_t decimal_mask = 1;
	uint8_t digits_count = 0;
	
	while (number2) {
		number2 /= 10;
		decimal_mask *= 10;
		digits_count++;
	}
	
	if (leading_zeros) {
		if (digits > digits_count) {
			digits_count = digits - digits_count;
			digits -= digits_count;
			memset(buffer, '0', digits_count);
			buffer += digits_count;
		}
	}
	else {
		if (digits > digits_count) digits = digits_count;
		if (number == 0) *buffer++ = '0';
	}
	
	while (digits--) {
		decimal_mask /= 10;
		*buffer++ = '0' + number / decimal_mask;
		number %= decimal_mask;
	}

	*buffer = '\0';
}

/*
void sprint_text(uint8_t * buffer, uint8_t position, const char * text) {
	uint8_t length = strlen(text);
	
	memset(buffer, 0, BUFF_SIZE);
	
	if ((0 == position) || (0 == length)) return;

	for (uint8_t i = 0, index = position - DIGIT_COUNT; i < DIGIT_COUNT; i++, index++)
		if (index < length) sprint_char(buffer, i, text[index]);
}

void sprint_char(uint8_t * buffer, uint8_t position, uint8_t character) {
	position = 0x80 >> position;
	if		(('0' <= character) && ('9' >= character))	character -= '0';
	else if	(('a' <= character) && ('z' >= character))	character += 10 - 'a';
	else if	('_' == character)							character = 36;
	else if	('-' == character)							character = 37;
	else if	('^' == character)							character = 38;
	else												character = 39;
	
	for (uint8_t i = 0, m = 1; i < SEGMENT_COUNT; i++, m <<= 1) {
		if (FONT[character] & m)	buffer[i] |= position;
		else						buffer[i] &= ~position;
	}
		
	max7219_send_buffer(buffer);
}

void max7219_send_buffer(uint8_t * buffer) {
	for (uint8_t i = 0; i < SEGMENT_COUNT; i++)
		max7219_send(i + 1, buffer[i]);
}
*/

void max7219_send(uint8_t addres, uint8_t value) {
	uint8_t i;

	for (i = 0x00; i < 0x04; i++)
		max7219_change(false, MAX7219_DATA_PIN_MASK);		// Dont care

	for (i = 0x08; i > 0x00; i >>= 1)
		max7219_change(addres & i, MAX7219_DATA_PIN_MASK);	// Adresse

	for (i = 0x80; i > 0x00; i >>= 1)
		max7219_change(value & i, MAX7219_DATA_PIN_MASK);	// Data

	max7219_pulse(MAX7219_LOAD_PIN_MASK);
}

void max7219_pulse(uint8_t mask) {
	max7219_change(true, mask);
	max7219_change(false, mask);
}

void max7219_change(bool is_high, uint8_t mask) {
	if (is_high)	PORTC |= mask;
	else			PORTC &= ~mask;
	if (MAX7219_DATA_PIN_MASK == mask)
		max7219_pulse(MAX7219_CLK_PIN_MASK);    			// clook pulse after data sent
}



