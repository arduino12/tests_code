#ifndef LED_EFFECT_H
#define LED_EFFECT_H

#include "led_strip.h"

typedef struct {
	uint16_t fade_ms = 0;
	uint16_t sleep_ms = 0;
	rgb_color_t rgb_color;
} effect_t;

typedef struct {
	uint16_t repetitions = 0;
	uint8_t effects_count = 0;
	uint8_t effect_index = 0;
	effect_t * effects = NULL;
} effects_t;

#define LED_EFFECT_INFINITE		(0xFFFF)

effects_t led_effect_effects[LED_STRIP_COUNT];


void led_effects_refresh() {
	effects_t * led_effects;
	millis();
	for (uint8_t i = 0; i < LED_STRIP_COUNT; i++) {
		led_effects = led_effect_effects + i;

	}
}

static effects_t * led_effect_get_led_effects(uint8_t index) {
	if (index >= LED_STRIP_COUNT)
		return NULL;
	return led_effect_effects + index;
}

void led_effects_clear(uint8_t index) {
	effects_t * led_effects = led_effect_get_led_effects(index);
	if (!led_effects)
		return;
	if (led_effects->effects)
		free(led_effects->effects)
	led_effects->repetitions = 0;
	led_effects->effect_index = 0;
	led_effects->effects_count = 0;
}

void led_effects_set_fix_ms(uint8_t index, uint16_t repetitions,
	uint16_t fade_ms, uint16_t sleep_ms, rgb_color_t * rgb_colors, 
	uint8_t effects_count) {
	
	effects_t * led_effects = led_effect_get_led_effects(index);
	if (!led_effects)
		return;
	if (led_effects->effects)
		free(led_effects->effects)
	
	led_effects->repetitions = repetitions;
	led_effects->effect_index = 0;
	led_effects->effects_count = effects_count;
	
	led_effects->effects = malloc(sizeof(effect_t) * effects_count);
	effect_t * led_effect = led_effects->effects;
	while(effects_count) {
		(*led_effect).fade_ms = fade_ms;
		(*led_effect).sleep_ms = sleep_ms;
		(*led_effect).rgb_color = *rgb_colors++;
		led_effect++;
		effects_count--;
	}
}

#endif /* LED_EFFECT_H */
