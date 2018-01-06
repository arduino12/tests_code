#include "buttons.h"


ButtonBitmask::ButtonBitmask() {}

ButtonBitmask::~ButtonBitmask() {
	end();
}

void ButtonBitmask::end() {
	_update_bitmask = NULL;
}

ButtonBitmask::ButtonBitmask(function_t update_bitmask) {
	ButtonBitmask::begin(update_bitmask);
}

void ButtonBitmask::begin(function_t update_bitmask) {
	_update_bitmask = update_bitmask;
	_press_up();
}

void ButtonBitmask::set_next_pulse_millis(uint32_t next_pulse_millis) {
	_next_pulse_millis = _down_millis + next_pulse_millis;
}

bool ButtonBitmask::_is_new_bitmask() {
	// Serial.println(F("ButtonBitmask::_is_new_bitmask()"));
	return button_bit;
}

bool ButtonBitmask::_is_old_bitmask() {
	// Serial.println(F("ButtonBitmask::_is_old_bitmask()"));
	return _button_bit;
}

bool ButtonBitmask::_is_bitmasks_match() {
	// Serial.println(F("ButtonBitmask::_is_bitmasks_match()"));
	return button_bit == _button_bit;
}

void ButtonBitmask::_match_bitmasks() {
	// Serial.println(F("ButtonBitmask::_match_bitmasks()"));
	_button_bit = button_bit;
}

void ButtonBitmask::__update_bitmask() {
	// Serial.println(F("ButtonBitmask::__update_bitmask()"));
	if (NULL == _update_bitmask) return;
	_update_bitmask();
}

void ButtonBitmask::update() {
	update(millis());
}

void ButtonBitmask::update(uint32_t cur_millis) {
	uint32_t delta_millis = cur_millis - _old_millis;
	
	__update_bitmask();
	if (_is_old_bitmask())	_down_millis += delta_millis;
	else					_up_millis += delta_millis;
	
	if		(is_released()) {
		_press_up();
	}
	else if	(_press_state == PRESS_STATE_LONG_PULSE) {
		_press_state = PRESS_STATE_LONG_PRESS;
	}
	
	if		(_press_state == PRESS_STATE_UP) {
		if (_is_new_bitmask()) {
			_down_millis = 0;
			_pulse_count = 0;
			_match_bitmasks();
			_press_state = PRESS_STATE_BOUNCE;
		}
	}
	else if	(_press_state == PRESS_STATE_BOUNCE) {
		if (!_is_bitmasks_match()) {
			_press_up();
		}
		else if (_down_millis >= short_press_millis) {
			_press_state = PRESS_STATE_SHORT_PRESS;
			on_short_press_func();
		}
	}
	else if	(_press_state == PRESS_STATE_SHORT_PRESS) {
		if (!_is_new_bitmask()) {
			_press_state = PRESS_STATE_SHORT_RELEASE;
			on_short_release_func();
		}
		else if (_down_millis >= long_press_millis) {
			set_next_pulse_millis(pulse_press_millis);
			_press_state = PRESS_STATE_LONG_PRESS;
			on_long_press_func();
		}
	}
	else if	(_press_state == PRESS_STATE_LONG_PRESS) {
		if (!_is_new_bitmask()) {
			_press_state = PRESS_STATE_LONG_RELEASE;
			on_long_release_func();
		}
		else if (_down_millis >= _next_pulse_millis) {
			set_next_pulse_millis(pulse_press_millis);
			_pulse_count++;
			_press_state = PRESS_STATE_LONG_PULSE;
			on_long_pulse_func();
		}
	}
	
	_old_millis = cur_millis;
}

void ButtonBitmask::_press_up() {
	_button_bit = false;
	_up_millis = 0;
	_down_millis = 0;
	_pulse_count = 0;
	_press_state = PRESS_STATE_UP;
}

uint32_t ButtonBitmask::get_up_millis() {
	return	_up_millis;
}

uint32_t ButtonBitmask::get_down_millis() {
	return	_down_millis;
}

uint32_t ButtonBitmask::get_pulse_count() {
	return	_pulse_count;
}

press_state_t ButtonBitmask::get_press_state() {
	return	_press_state;
}

bool ButtonBitmask::is_up() {
	return	!is_down();
}

bool ButtonBitmask::is_down() {
	return	is_pressed() || (_press_state == PRESS_STATE_BOUNCE);
}

bool ButtonBitmask::is_pressed() {
	return	(_press_state == PRESS_STATE_SHORT_PRESS) ||
			(_press_state == PRESS_STATE_LONG_PRESS) ||
			(_press_state == PRESS_STATE_LONG_PULSE);
}

bool ButtonBitmask::is_released() {
	return	(_press_state == PRESS_STATE_SHORT_RELEASE) ||
			(_press_state == PRESS_STATE_LONG_RELEASE);
}

bool ButtonBitmask::is_pulsed() {
	return	(_press_state == PRESS_STATE_LONG_PULSE);
}


ButtonsBitmask::ButtonsBitmask() {}

ButtonsBitmask::~ButtonsBitmask() {
	end();
}

void ButtonsBitmask::end() {
	ButtonBitmask::end();
	if (NULL != buttons_bitmask)
		free(buttons_bitmask);
	if (NULL != _buttons_bitmask)
		free(_buttons_bitmask);
	buttons_bitmask = NULL;
}

ButtonsBitmask::ButtonsBitmask(function_t update_bitmask, uint8_t buttons_count) {
	ButtonsBitmask::begin(update_bitmask, buttons_count);
}

void ButtonsBitmask::begin(function_t update_bitmask, uint8_t buttons_count) {
	// Serial.println("ButtonsBitmask::begin()");
	ButtonBitmask::begin(update_bitmask);
	_buttons_count = buttons_count;
	_buttons_bitmask_size = (_buttons_count + 7) / 8;
	buttons_bitmask = malloc(_buttons_bitmask_size);
	_buttons_bitmask = malloc(_buttons_bitmask_size);
}

bool ButtonsBitmask::_is_new_bitmask() {
	// Serial.println("ButtonsBitmask::_is_new_bitmask()");
	for (uint8_t i = 0; i < _buttons_bitmask_size; i++)
		if (buttons_bitmask[i])
			return true;
	return false;
}

bool ButtonsBitmask::_is_old_bitmask() {
	// Serial.println("ButtonsBitmask::_is_old_bitmask()");
	for (uint8_t i = 0; i < _buttons_bitmask_size; i++)
		if (_buttons_bitmask[i])
			return true;
	return false;
}

bool ButtonsBitmask::_is_bitmasks_match() {
	// Serial.println("ButtonsBitmask::_match_bitmasks()");
	return 0 == memcmp(_buttons_bitmask, buttons_bitmask, _buttons_bitmask_size);
}

void ButtonsBitmask::_match_bitmasks() {
	// Serial.println("ButtonsBitmask::_match_bitmasks()");
	memcpy(_buttons_bitmask, buttons_bitmask, _buttons_bitmask_size);
}

void ButtonsBitmask::_press_up() {
	// Serial.println("ButtonsBitmask::_press_up()");
	ButtonBitmask::_press_up();
	memset(_buttons_bitmask, 0, _buttons_bitmask_size);
}

uint8_t * ButtonsBitmask::_malloc_and_copy(uint8_t * src, uint8_t size) {
	// Serial.println("ButtonsBitmask::_malloc_and_copy()");
	uint8_t * ret = malloc(size);
	memcpy(ret, src, size);
	return ret;
}

uint8_t ButtonsBitmask::get_buttons_count() {
	return _buttons_count;
}

uint8_t ButtonsBitmask::get_button_index() {
	// __update_bitmask();
	for (uint8_t i = 0; i < _buttons_count; i++)
		if (1 == (buttons_bitmask[i / 8] >> (i % 8)))
			return i + 1;
	return 0;
}


Keys::Keys() {}

Keys::~Keys() {
	end();
}

void Keys::end() {
	ButtonsBitmask::end();
	if (NULL != _buttons_pins)
		free(_buttons_pins);
	_buttons_pins = NULL;
}

Keys::Keys(uint8_t * pins, uint8_t pins_count) {
	Keys::begin(pins, pins_count);
}

void Keys::begin(uint8_t * pins, uint8_t pins_count) {
	ButtonsBitmask::begin(NULL, pins_count);
	// ButtonsBitmask::begin((function_t)&Keys::__update_bitmask, pins_count);
	// ButtonsBitmask::begin(empty_function, pins_count);
	// ButtonsBitmask::_update_bitmask = (function_t)&Keys::__update_bitmask;
	_buttons_pins = ButtonsBitmask::_malloc_and_copy(pins, ButtonsBitmask::_buttons_count);
	for (uint8_t i = 0; i < ButtonsBitmask::_buttons_count; i++)
		pinMode(_buttons_pins[i], INPUT_PULLUP);
}

void Keys::__update_bitmask() {
	memset(buttons_bitmask, 0, ButtonsBitmask::_buttons_bitmask_size);
	for (uint8_t i = 0; i < ButtonsBitmask::_buttons_count; i++)
		if (!digitalRead(_buttons_pins[i]))
			buttons_bitmask[i / 8] |= 1 << (i % 8);
}


Keypad::Keypad() {}

Keypad::~Keypad() {
	end();
}

void Keypad::end() {
	ButtonsBitmask::end();
	if (NULL != _rows_pins)
		free(_rows_pins);
	if (NULL != _columns_pins)
		free(_columns_pins);
	_rows_pins = NULL;
	_columns_pins = NULL;
}

Keypad::Keypad(uint8_t * rows_pins, uint8_t * columns_pins, uint8_t rows_count, uint8_t columns_count) {
	Keypad::begin(rows_pins, columns_pins, rows_count, columns_count);
}

void Keypad::begin(uint8_t * rows_pins, uint8_t * columns_pins, uint8_t rows_count, uint8_t columns_count) {
	ButtonsBitmask::begin(NULL, rows_count * columns_count);
	// ButtonsBitmask::begin((function_t)&Keypad::__update_bitmask, rows_count * columns_count);
	// ButtonsBitmask::begin(empty_function, rows_count * columns_count);
	// ButtonsBitmask::_update_bitmask = (function_t)&Keypad::__update_bitmask;
	_rows_count = rows_count;
	_columns_count = columns_count;
	_rows_pins = ButtonsBitmask::_malloc_and_copy(rows_pins, _rows_count);
	_columns_pins = ButtonsBitmask::_malloc_and_copy(columns_pins, _columns_count);

	// set all row pins to logic "Z"
	for (uint8_t i = 0; i < _rows_count; i++)
		pinMode(_rows_pins[i], INPUT);
	// pull all columns pins to logic "1"
	for (uint8_t i = 0; i < _columns_count; i++)
		pinMode(_columns_pins[i], INPUT_PULLUP);
	// Serial.println("Keypad.begin");
}

void Keypad::__update_bitmask() {
	// Serial.println('.');
	memset(buttons_bitmask, 0, ButtonsBitmask::_buttons_bitmask_size);
	for (uint8_t r = 0, i = 0; r < _rows_count; r++) {
		pinMode(_rows_pins[r], OUTPUT);
		digitalWrite(_rows_pins[r], LOW);
		for (uint8_t c = 0; c < _columns_count; c++, i++) {
			if (!digitalRead(_columns_pins[c])) {
				buttons_bitmask[i / 8] |= 1 << (i % 8);		
				// Serial.println(i);
			}
		}
		pinMode(_rows_pins[r], INPUT);
	}
}
