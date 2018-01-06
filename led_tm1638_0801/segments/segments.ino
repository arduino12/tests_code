#include <TM1638.h>

TM1638 tm1638;							// global object of the module driver

void setup() {
	Serial.begin(115200);				// initialize serial to 115200 baud
	Serial.println(F("setup()"));		// use serial for debug printing
	
	tm1638.begin(A2, A0, A1);			// data, clock, strobe
	
	tm1638.clearLeds();					// turn off all red-green LEDs
	tm1638.clearSegments();				// turn off all digits and dots
	
	tm1638.sendChars("3.14");			// bad because full digit shows only '.'
	delay(1000);
	tm1638.clearSegments(0, 4);			// clear only the used digits
	tm1638.printDouble(3.14);			// solution..!
	delay(1000);
	
	char buffer[5];						// 1234 = 4 digits + null = 5
	tm1638.itoa2(buffer, 1234);			// print the number to the buffer
	for (int8_t i = SEGMENTS_COUNT; i > -SEGMENTS_COUNT; i--) {
		tm1638.sendChars(buffer, i);	// send the buffer to display digits
		delay(300);
		tm1638.clearSegments(i, 4);		// clear only the used digits
	}
	
	tm1638.printIntegr(28);				// 28
	tm1638.sendSegment(0x63, 2);		// 28o
	tm1638.sendChar('C', 3);			// 28oC
	delay(1500);
	
	tm1638.sendChars("bad...");			// string saved in DRAM
	delay(1500);
	tm1638.sendChars_P(PSTR("good !"));	// string is stored in FLASH
	delay(1500);
	
	for (int8_t i = 0; i < SEGMENTS_COUNT; i++) {
		tm1638.sendLed((led_color_t)(i % 2 + 1), i);
		delay(300);
	}
	delay(1500);
	
	Serial.println(F("loop()"));		// going to endless loop
}

void loop() {
	static int32_t i = -1000;
	tm1638.printIntegr(i++, 0, SEGMENTS_COUNT, true); // with leading zeros
	if ((i % 100) == 0)
		tm1638.sendLeds(random(0xFFFF));				// send random led pattern - 2 bits = 1 led color
	delay(1);
}
