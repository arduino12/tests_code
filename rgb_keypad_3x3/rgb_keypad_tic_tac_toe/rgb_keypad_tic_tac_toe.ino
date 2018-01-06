//{ includes:
#include "globals.h"
#include "led_strip.h"
#include <Buttons.h>
//}

//{ defines:
#define LED_MASK_X			(0b101010101)
#define LED_MASK_O			(0b010101010)
#define LED_MASK_SQUARE		(0b111101111)
//}

//{ globals
Keypad keypad;
uint16_t red_leds_mask		= 0b000000000;
uint16_t green_leds_mask	= 0b000000000;
bool player;
//}

//{ helpers

void refresh() {
	led_strip_refresh();
	delay(1);
}

void delay_with_refresh(uint32_t ms) {
	uint32_t start_ms = millis();
	while ((start_ms + ms) > millis()) refresh();
}

bool is_three_sequence(uint16_t mask) {
	/* did the given mask won the game? */
	if ((mask & 0b000000111) == 0b000000111) return true;
	if ((mask & 0b000111000) == 0b000111000) return true;
	if ((mask & 0b111000000) == 0b111000000) return true;
	if ((mask & 0b100100100) == 0b100100100) return true;
	if ((mask & 0b010010010) == 0b010010010) return true;
	if ((mask & 0b001001001) == 0b001001001) return true;
	if ((mask & 0b001010100) == 0b001010100) return true;
	if ((mask & 0b100010001) == 0b100010001) return true;
	return false;
}

void keypad_short_press() {
	// uint16_t mask = keypad.getButtonsBitmask();
	uint8_t index = keypad.getButtonIndex();
	
	if (!index--) return;
	uint16_t mask = (uint16_t)1 << index;
	/* check that the led under the cruser is empty */
	if (((mask & red_leds_mask) != 0) || ((mask & green_leds_mask) != 0))
		return;
	/* get pointer to the current mask (player = true - red, else green) */
	uint16_t * player_mask = (player ? &red_leds_mask : &green_leds_mask);
	/* turn on the led under the cruser */
	*player_mask |= mask;
	led_strip_set_color(index, (player ? COLOR_RED : COLOR_GREEN));
	/* did the player won? */
	if (is_three_sequence(*player_mask)) {
		delay_with_refresh(500);
		// delay_with_refresh(1500);
		game_restert();
	}
	/* did all leds are on? */
	else if ((red_leds_mask | green_leds_mask) == 0b111111111) {
		delay_with_refresh(500);
		// delay_with_refresh(1500);
		game_restert();
	}
	/* go on... */
	else {
		player = !player;
	}
}

void game_restert() {
	red_leds_mask = 0;
	green_leds_mask = 0;
	player = false;
	led_strip_set_colors(COLOR_BLACK);
}


//}

//{ setup and loop
void setup() {
	/* initialize the debug UART */
	DEBUG_INIT;
	DEBUG_PRINTLN(F("setup"));
	/* initialize the RGB LED strip */
	led_strip_init();

	uint8_t row_pins[]		= {A0, A1, A2};
	uint8_t column_pins[]	= {8, 9, 10};
	
	keypad.begin(
		row_pins,
		column_pins,
		sizeof(row_pins),
		sizeof(column_pins)
	);
	keypad.setPressMillis(
		20,
		1000,
		800
	);
	keypad.setShortPressFunc(
		keypad_short_press
	);
	
	/* start infinity loop */
	DEBUG_PRINTLN(F("loop"));
	game_restert();
}

void loop() {
	/* update the keypad states and call events if needed */
	keypad.update();
	refresh();
}
//}
