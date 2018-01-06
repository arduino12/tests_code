#ifndef LED_STRIP_H
#define LED_STRIP_H

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_color_t;

#define LED_STRIP_COUNT			(9)
#define LED_STRIP_DATA_PIN		(A3)
#define USE_GAMMA_CORRECTION

#ifdef USE_GAMMA_CORRECTION
	#define COLOR_BLACK			(0x0000)
	#define COLOR_RED			(0x001F)
	#define COLOR_GREEN			(0x03E0)
	#define COLOR_BLUE			(0xEB00)
	#define COLOR_YELLOW		(0x03FF)
	#define COLOR_CYAN			(0xEFE0)
	#define COLOR_MAGENTA		(0xEB1F)
	#define COLOR_WHITE			(0xEFFF)

	/* 5 to 8 bits gamma correction */
	const uint8_t gamma_5_to_8[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x11, 0x15, 0x1A, 0x1F, 0x25, 0x2B,
		0x32, 0x3A, 0x42, 0x4B, 0x55, 0x60, 0x6C, 0x78, 0x86, 0x94, 0xA3, 0xB4, 0xC5, 0xD7, 0xEB, 0xFF,
	};
	#define COLOR_VALUE(value)	gamma_5_to_8[value]
	#define COLOR_CHANNEL_BITS	(5)
	typedef uint16_t			color_t;
#else
	#define COLOR_BLACK			(0x000000)
	#define COLOR_RED			(0x0000FF)
	#define COLOR_GREEN			(0x00FF00)
	#define COLOR_BLUE			(0xFF0000)
	#define COLOR_YELLOW		(0x00FFFF)
	#define COLOR_CYAN			(0xFFFF00)
	#define COLOR_MAGENTA		(0xFF00FF)
	#define COLOR_WHITE			(0xFFFFFF)
	
	#define COLOR_VALUE(value)
	#define COLOR_CHANNEL_BITS	(8)
	typedef uint32_t			color_t;
#endif

#define COLOR_CHANNEL_COUNT		(1 << COLOR_CHANNEL_BITS)
#define COLOR_CHANNEL_MASK		(COLOR_CHANNEL_COUNT - 1)

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
			:	"r" (COLOR_VALUE(value)), "I" (_SFR_IO_ADDR(PORTC)), \
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

color_t led_strip_get_color(uint8_t index) {
	if (index >= LED_STRIP_COUNT)
		return 0;
	rgb_color_t * rgb_color = led_strip_buffer + index;
	color_t color = rgb_color->b;
	color <<= COLOR_CHANNEL_BITS;
	color |= rgb_color->g;
	color <<= COLOR_CHANNEL_BITS;
	color |= rgb_color->r;
	return color;
}

void to_rgb_color(rgb_color_t * rgb_color, color_t color) {
	rgb_color->r = color & COLOR_CHANNEL_MASK;
	rgb_color->g = (color >> COLOR_CHANNEL_BITS) & COLOR_CHANNEL_MASK;
	rgb_color->b = (color >> (COLOR_CHANNEL_BITS * 2)) & COLOR_CHANNEL_MASK;
}

void led_strip_set_color(uint8_t index, color_t color) {
	if (index >= LED_STRIP_COUNT)
		return;
	to_rgb_color(led_strip_buffer + index, color);
}

void led_strip_set_colors() {
	for (uint8_t i = 1; i < LED_STRIP_COUNT; i++)
		led_strip_buffer[i] = led_strip_buffer[0];
}

void led_strip_set_colors(color_t color) {
	to_rgb_color(led_strip_buffer, color);
	led_strip_set_colors();
}

/* initialize smart pixel RGB LEDs */
void led_strip_init() {
	pinMode(LED_STRIP_DATA_PIN, OUTPUT);
	led_strip_refresh();
}

#endif /* LED_STRIP_H */
