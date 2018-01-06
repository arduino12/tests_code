#ifndef LED_STRIP_H
#define LED_STRIP_H

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_color_t;

#define LED_STRIP_COUNT			(9)
#define LED_STRIP_DATA_PIN		(A3)

#define COLOR_BLACK				(0x000000)
#define COLOR_RED				(0x0000FF)
#define COLOR_GREEN				(0x00FF00)
#define COLOR_BLUE				(0xFF0000)
#define COLOR_YELLOW			(0x00FFFF)
#define COLOR_CYAN				(0xFFFF00)
#define COLOR_MAGENTA			(0xFF00FF)
#define COLOR_WHITE				(0xFFFFFF)

rgb_color_t led_strip_buffer[LED_STRIP_COUNT];


/* refresh the smart pixel RGB LEDs by setting their color to match the led_strip_buffer */
void led_strip_refresh() {
	/* disable global interrupts */
	cli();
	/* 0x08 and PORTC hard cooded for STRIP_LED_DATA_PIN = A3 */
	uint8_t value, ctr, mask_low, mask_high = 0x08;
	mask_low = ~mask_high & PORTC;
	mask_high |= PORTC;
	
	#define SEND_LED_BYTE \
		asm volatile ( \
			"		ldi %0,8	\n\t" \
			"loop%=:out %2, %3	\n\t" \
			"lsl	%1			\n\t" \
			"dec	%0			\n\t" \
			"		rjmp .+0	\n\t" \
			"		brcs .+2	\n\t" \
			"		out %2,%4	\n\t" \
			"		rjmp .+0	\n\t" \
			"		nop			\n\t" \
			"		out %2,%4	\n\t" \
			"		breq end%=	\n\t" \
			"		rjmp .+0	\n\t" \
			"		rjmp .+0	\n\t" \
			"		rjmp .+0	\n\t" \
			"		rjmp loop%=	\n\t" \
			"end%=:				\n\t" \
			:	"=&d" (ctr)			  \
			:	"r" (value), "I" (_SFR_IO_ADDR(PORTC)), \
			"r" (mask_high), "r" (mask_low) \
		)

	/* smart pixel RGB LEDs order: green red blue */
	for (uint8_t i = 0; i < LED_STRIP_COUNT; i++) {
		value = led_strip_buffer[i].g;
		SEND_LED_BYTE;
		value = led_strip_buffer[i].r;
		SEND_LED_BYTE;
		value = led_strip_buffer[i].b;
		SEND_LED_BYTE;
	}
	
	/* enable global interrupts */
	sei();
}

uint32_t led_strip_get_color(uint8_t index) {
	if (index >= LED_STRIP_COUNT)
		return 0;
	rgb_color_t * rgb_color = led_strip_buffer + index;
	uint32_t color = rgb_color->b;
	color <<= 8;
	color |= rgb_color->g;
	color <<= 8;
	color |= rgb_color->r;
	return color;
}

void to_rgb_color(rgb_color_t * rgb_color, uint32_t color) {
	rgb_color->r = color;
	rgb_color->g = color >> 8;
	rgb_color->b = color >> 16;
}

void led_strip_set_color(uint8_t index, uint32_t color) {
	if (index >= LED_STRIP_COUNT)
		return;
	to_rgb_color(led_strip_buffer + index, color);
}

void led_strip_set_colors(uint32_t color) {
	to_rgb_color(led_strip_buffer, color);
	for (uint8_t i = 1; i < LED_STRIP_COUNT; i++)
		led_strip_buffer[i] = led_strip_buffer[0];
}

/* initialize smart pixel RGB LEDs */
void led_strip_init() {
	pinMode(LED_STRIP_DATA_PIN, OUTPUT);
	led_strip_refresh();
}

#endif /* LED_STRIP_H */
