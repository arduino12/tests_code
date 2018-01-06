#ifndef WS2812_H
#define WS2812_H

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_color_t;

#define WS2812_COUNT					(9)
#define WS2812_DATA_PIN					(A3)
#define USE_GAMMA_CORRECTION

#ifdef USE_GAMMA_CORRECTION
	#define WS2812_COLOR_BLACK			(0x0000)
	#define WS2812_COLOR_RED			(0x001F)
	#define WS2812_COLOR_GREEN			(0x03E0)
	#define WS2812_COLOR_BLUE			(0xEB00)
	#define WS2812_COLOR_YELLOW			(0x03FF)
	#define WS2812_COLOR_CYAN			(0xEFE0)
	#define WS2812_COLOR_MAGENTA		(0xEB1F)
	#define WS2812_COLOR_WHITE			(0xEFFF)

	/* 5 to 8 bits gamma correction */
	const uint8_t gamma_5_to_8[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x11, 0x15, 0x1A, 0x1F, 0x25, 0x2B,
		0x32, 0x3A, 0x42, 0x4B, 0x55, 0x60, 0x6C, 0x78, 0x86, 0x94, 0xA3, 0xB4, 0xC5, 0xD7, 0xEB, 0xFF,
	};

	#define WS2812_COLOR_VALUE(value)	(gamma_5_to_8[value])
	#define WS2812_COLOR_CHANNEL_BITS	(5)
	typedef uint16_t color_t;
#else
	#define WS2812_COLOR_BLACK			(0x000000)
	#define WS2812_COLOR_RED			(0x0000FF)
	#define WS2812_COLOR_GREEN			(0x00FF00)
	#define WS2812_COLOR_BLUE			(0xFF0000)
	#define WS2812_COLOR_YELLOW			(0x00FFFF)
	#define WS2812_COLOR_CYAN			(0xFFFF00)
	#define WS2812_COLOR_MAGENTA		(0xFF00FF)
	#define WS2812_COLOR_WHITE			(0xFFFFFF)
	
	#define WS2812_COLOR_VALUE(value)	(value)
	#define WS2812_COLOR_CHANNEL_BITS	(8)
	typedef uint32_t color_t;
#endif

#define WS2812_COLOR_CHANNEL_COUNT		(1 << WS2812_COLOR_CHANNEL_BITS)
#define WS2812_COLOR_CHANNEL_MASK		(WS2812_COLOR_CHANNEL_COUNT - 1)

rgb_color_t ws2812_buffer[WS2812_COUNT];


/* refresh the smart pixel RGB LEDs by setting their color to match the ws2812_buffer */
void ws2812_refresh() {
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
			:	"r" (WS2812_COLOR_VALUE(value)), "I" (_SFR_IO_ADDR(PORTC)), \
			"r" (mask_high), "r" (mask_low) \
		)

	/* smart pixel RGB LEDs order: green red blue */
	for (uint8_t i = 0; i < WS2812_COUNT; i++) {
		value = ws2812_buffer[i].g;
		SEND_LED_BYTE;
		value = ws2812_buffer[i].r;
		SEND_LED_BYTE;
		value = ws2812_buffer[i].b;
		SEND_LED_BYTE;
	}
	
	/* enable global interrupts */
	sei();
}

color_t ws2812_get_color(uint8_t index) {
	if (index >= WS2812_COUNT)
		return 0;
	rgb_color_t * rgb_color = ws2812_buffer + index;
	color_t color = rgb_color->b;
	color <<= WS2812_COLOR_CHANNEL_BITS;
	color |= rgb_color->g;
	color <<= WS2812_COLOR_CHANNEL_BITS;
	color |= rgb_color->r;
	return color;
}

void to_rgb_color(rgb_color_t * rgb_color, color_t color) {
	rgb_color->r = color & WS2812_COLOR_CHANNEL_MASK;
	rgb_color->g = (color >> WS2812_COLOR_CHANNEL_BITS) & WS2812_COLOR_CHANNEL_MASK;
	rgb_color->b = (color >> (WS2812_COLOR_CHANNEL_BITS * 2)) & WS2812_COLOR_CHANNEL_MASK;
}

void ws2812_set_color(uint8_t index, color_t color) {
	if (index >= WS2812_COUNT)
		return;
	to_rgb_color(ws2812_buffer + index, color);
}

void ws2812_set_colors() {
	for (uint8_t i = 1; i < WS2812_COUNT; i++)
		ws2812_buffer[i] = ws2812_buffer[0];
}

void ws2812_set_colors(color_t color) {
	to_rgb_color(ws2812_buffer, color);
	ws2812_set_colors();
}

/* initialize smart pixel RGB LEDs */
void ws2812_init() {
	pinMode(WS2812_DATA_PIN, OUTPUT);
	ws2812_refresh();
}

#endif /* WS2812_H */
