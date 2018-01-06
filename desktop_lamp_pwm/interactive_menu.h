#ifndef INTERACTIVE_MENU_H
#define INTERACTIVE_MENU_H

#include "pca9685.h"

/* max amount of bytes for a command */
#define COMMAND_BUFFER_SIZE			(254)
/* max amount of milliseconds for command buffer to grow */
#define COMMAND_TIMEOUT_MS			(150)

#define BRIGHTNESS_LEVELS			(4096)
#define MAX_BRIGHTNESS				(BRIGHTNESS_LEVELS - 1)

#define HUE_LEVELS					(BRIGHTNESS_LEVELS * 6)
#define HUE_BLACK					(HUE_LEVELS + 0)
#define HUE_GRAY					(HUE_LEVELS + 1)
#define HUE_WHITE					(HUE_LEVELS + 2)

struct {
	char buffer[COMMAND_BUFFER_SIZE];
	uint8_t buffer_index = 0;
	uint32_t timeout = 0;
} command;

struct {
	uint16_t r = 0;
	uint16_t g = 0;
	uint16_t b = 0;
	uint16_t w = 0;
} lamp_color;

/* prints human readable commands help */
void print_help() {
	Serial.println(F(
		"H <>\n"
		"    Print help\n"
		"I <leds_bitmask[0-32K]>\n"
		"    Set LEDs to the given bitmask\n"
	));
}

/* reads a number from the command buffer */
uint16_t _interactive_menu_read_16(uint16_t number) {
	uint16_t i = number;
	sscanf(command.buffer + 2, "%d", &i);
	return i;
}

void update_color() {
	pca9685_write(4, lamp_color.r);
	pca9685_write(5, lamp_color.w);
	pca9685_write(6, lamp_color.g);
	pca9685_write(7, lamp_color.b);
}

void set_rgb_color(uint16_t r, uint16_t g, uint16_t b) {
	lamp_color.r = r;
	lamp_color.w = 0;
	lamp_color.g = g;
	lamp_color.b = b;
	update_color();
}

void set_hue_color(uint16_t h) {
	uint16_t t;
	if (h < HUE_LEVELS) {
		t = h % BRIGHTNESS_LEVELS;
		switch (h / BRIGHTNESS_LEVELS) {
			case 0: set_rgb_color(MAX_BRIGHTNESS, t, 0); break;
			case 1: set_rgb_color(MAX_BRIGHTNESS - t, MAX_BRIGHTNESS, 0); break;
			case 2: set_rgb_color(0, MAX_BRIGHTNESS, t); break;
			case 3: set_rgb_color(0, MAX_BRIGHTNESS - t, MAX_BRIGHTNESS); break;
			case 4: set_rgb_color(t, 0, MAX_BRIGHTNESS); break;
			case 5: set_rgb_color(MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS - t); break;
		}
	}
	else {
		t = (h - HUE_LEVELS) % MAX_BRIGHTNESS;
		set_rgb_color(t, t, t);
	}
}

/* must be called repeatedly from loop, this is our busy loop */
void interactive_menu_update() {
	uint32_t cur_millis = millis();
	uint16_t i;
	static uint16_t hue = HUE_LEVELS;

	/* is it the time for a screen saver? */
	if (cur_millis > command.timeout) {
		if (command.buffer_index)
			command.buffer_index = 0;
	}

	if (hue < HUE_LEVELS) {
		set_hue_color(hue);
		hue++;
		if (hue >= HUE_LEVELS)
			hue = 0;
	}
	
	/* got a command? */
	if (Serial.available()) {
		if (!command.buffer_index)
			command.timeout = cur_millis + COMMAND_TIMEOUT_MS;

		command.buffer_index += Serial.readBytes(command.buffer + command.buffer_index,
			sizeof(command.buffer) - command.buffer_index);

		i = command.buffer[command.buffer_index - 1];
		if (command.buffer_index >= 2 && i == '\n' || i == '\r') {
			i = command.buffer[command.buffer_index - 2];
			i = ((i == '\n' || i == '\r') ? 3 : 2);
			while (i--)
				command.buffer[command.buffer_index - i] = '\0';

			switch (command.buffer[0]) {
				case 'H':
					print_help();
				break;
				case 'A':
					hue = (hue >= HUE_LEVELS ? 0 : HUE_LEVELS);
					set_rgb_color(0, 0, 0);
				break;
				case 'I':
					// sscanf(command.buffer + 2, "%d %d %d %d", &(lamp_color.r), &(lamp_color.g), &(lamp_color.b), &(lamp_color.w));
					// Serial.print(F("rgb = ")); Serial.println(i, BIN);
					update_color();
				break;
			}
			command.buffer_index = 0;
		}
	}
}

#endif /* INTERACTIVE_MENU_H */
