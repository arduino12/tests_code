/*
TODO:
	Games: tic-tac-two, simon
	Efects: patterns, random
	Laser Projector:
		each press on the sides starts self draw
		each press on the middle starts auto draw
		long press on the middle starts game!
		cool RGB patterns while draw
		
	Effects:
		generic fade: 
			arguments:
				array of colors to fade
				fade step 1 to COLOR_CHANNEL_MASK
				blink 0 = no blink, 256 = 10 seconds
				
*/

#include "buttons.h"
#include "ws2812.h"

#define LED_MASK_X			(0b101010101)
#define LED_MASK_O			(0b010101010)
#define LED_MASK_SQUARE		(0b111101111)


uint16_t red_leds_mask		= 0b000000000;
uint16_t green_leds_mask	= 0b000000000;
bool player;

Keypad keypad;

void refresh() {
	// led_effects_refresh()
	ws2812_refresh();
	delay(1);
}

void delay_with_refresh(uint32_t ms) {
	uint32_t start_ms = millis();
	while ((start_ms + ms) > millis())
		refresh();
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
	// uint8_t index = random(1, 10);
	uint8_t index = keypad.get_button_index();
	Serial.println(F("."));
	if (!index--) return;
	uint16_t mask = (uint16_t)1 << index;
	/* check that the led under the cruser is empty */
	if (((mask & red_leds_mask) != 0) || ((mask & green_leds_mask) != 0))
		return;
	/* get pointer to the current mask (player = true - red, else green) */
	uint16_t * player_mask = (player ? &red_leds_mask : &green_leds_mask);
	/* turn on the led under the cruser */
	*player_mask |= mask;
	ws2812_set_color(index, (player ? WS2812_COLOR_RED : WS2812_COLOR_GREEN));
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
	ws2812_set_colors(WS2812_COLOR_BLACK);
}

void setup() {
	/* initialize the debug UART */
	Serial.begin(115200);
	Serial.println(F("setup"));
	/* initialize the RGB LED strip */
	ws2812_init();
	/* initialize the keypad buttons */
	uint8_t row_pins[]		= {A0, A1, A2};
	uint8_t column_pins[]	= {8, 9, 10};
	keypad.begin(row_pins, column_pins, sizeof(row_pins), sizeof(column_pins));
	keypad.on_short_press_func = &keypad_short_press;
	/* white fade */
	for (uint8_t j = 0; j < 2; j++)
		for (uint8_t i = 0; i < WS2812_COLOR_CHANNEL_COUNT; i++) {
			(*ws2812_buffer).r = i;
			(*ws2812_buffer).g = i;
			(*ws2812_buffer).b = i;
			ws2812_set_colors();
			delay_with_refresh(20);
		}
	/* start infinity loop */
	Serial.println(F("loop"));
	game_restert();
}

void loop() {
	/* update the keypad states and call events if needed */
	// if (random(100) == 0)
		// keypad_short_press();
	keypad.update();
	refresh();
}
