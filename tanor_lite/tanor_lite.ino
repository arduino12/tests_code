/*
	/-----------\
	| 8 8 : 8 8 |
	\-----------/
*/

#include <Time.h>
#include <Wire.h>
#include <DS3232RTC.h>


#define DOT_ON_COLOR		(255)
#define DOT_OFF_COLOR		(0)
#define DOT_U_LED_INDEX		(56)
#define DOT_D_LED_INDEX		(57)
#define DOT_U_MASK			(0x01)
#define DOT_D_MASK			(0x02)
#define DOT_BOTH_MASK		(DOT_U_MASK | DOT_D_MASK)

#define MIN_PER_HOUR		(60)
#define MIN_PER_DAY			(SECS_PER_DAY / SECS_PER_MIN)

#define CLOCK_DOT_MS		(650)


static const uint8_t FONT[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,	// 0,1,2,3,4,5,6,7,8,9
	// 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76,	0x30, 0x0E,	// a,b,c,d,e,f,g,h,i,j
	// 0x00, 0x38, 0x00, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78,	// k,l,m,n,o,p,q,r,s,t
	// 0x3E, 0x1C, 0x00, 0x00, 0x6E, 0x5C, 0x08, 0x40, 0x01, 0x00,	// u,v,w,x,y,z,_,-,^, 
};

void draw_digit(uint8_t index, uint8_t bitmask) {
	//TODO: set pixels LEDs...
}

uint16_t get_current_time() {
	return (now() % SECS_PER_DAY) / SECS_PER_MIN;
}

void print_dots(uint8_t dots) {
	pixel.set_led(DOT_U_LED_INDEX, (dots & DOT_U_MASK ? DOT_ON_COLOR : DOT_OFF_COLOR));
	pixel.set_led(DOT_D_LED_INDEX, (dots & DOT_D_MASK ? DOT_ON_COLOR : DOT_OFF_COLOR));
}

void print_time(uint16_t minutes) {
	uint8_t hours = minutes / MIN_PER_HOUR;
	minutes %= MIN_PER_HOUR;

	draw_digit(0, (hours > 9 ? FONT[hours / 10 % 10] : 0));
	draw_digit(1, FONT[hours % 10]);
	draw_digit(2, FONT[minutes / 10 % 10]);
	draw_digit(3, FONT[minutes % 10]);
}

void setup() {
	setSyncProvider(RTC.get);
	/* test print_time */
	for (uint8_t i = 0; i < 10; i++) {
		print_time(i);
		delay(1000);
	}
}

void loop() {
	uint8_t dots = 0;
	uint16_t cur_time, old_time = 0;
	uint32_t cur_ms, old_ms = 0xFFFF, delta_ms;
	
	for (;;) {
		cur_ms = millis();
		delta_ms = cur_ms - old_ms;
		
		if (CLOCK_DOT_MS < delta_ms) {
			old_ms = cur_ms;
			cur_time = get_current_time();
			if (old_time != cur_time) {
				print_time(cur_time);
				old_time = cur_time;
			}
			print_dots(dots);
			dots = ~dots;
		}

		delay(1);
	}
}
