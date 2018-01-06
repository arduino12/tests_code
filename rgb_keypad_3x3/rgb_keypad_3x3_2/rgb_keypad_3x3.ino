
//{ includes:
#include "globals.h"
#include "led_strip.h"
#include "dot_display.h"
#include "text_view.h"
#include "font_5x7.h"
#include "text.h"
//}

//{ defines:

//}

//{ globals

// TextView tv;

//}

//{ helpers
//}

//{ setup and loop
void setup() {
	/* initialize the debug UART */
	DEBUG_INIT;
	DEBUG_PRINTLN(F("setup"));
	/* initialize the RGB LED strip */
	led_strip_init();
	/* initialize the display */
	dot_display_init();
	/* start infinity loop */
	DEBUG_PRINTLN(F("loop"));
	
	tv.begin(font_5x7, "hello");
	
	
	led_strip_set_color(0, COLOR_BLACK);
	led_strip_set_color(1, COLOR_BLACK);
	led_strip_set_color(2, COLOR_BLACK);
	led_strip_set_color(3, COLOR_BLACK);
	led_strip_set_color(4, COLOR_BLACK);
	led_strip_set_color(5, COLOR_BLACK);
	led_strip_set_color(6, COLOR_BLACK);
	led_strip_set_color(7, COLOR_BLACK);
	led_strip_set_color(8, COLOR_BLACK);
	led_strip_set_color(9, COLOR_BLACK);
	// print_text("12", 0);
	// print_text("hello world !!!", 0);
	// print_char('$', 0);
	print_text("hello world", 0);
	dot_display_refresh();
	delay(1000);
}

void loop() {
	static uint8_t i = 0;
	// memset(dot_display_buffer, i, DISPLAY_WIDTH);
	// dot_display_refresh();
	print_text(hello_hebrew, i);
	dot_display_refresh();
	if (i >= DISPLAY_WIDTH) i = 0;
	else i+= FONT_WIDTH;
	led_strip_refresh();
	delay(100);
}
//}
