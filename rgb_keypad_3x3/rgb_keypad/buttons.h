#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>


#define DEFAULT_SHORT_PRESS_MILLIS	(50)
#define DEFAULT_LONG_PRESS_MILLIS	(1000)
#define DEFAULT_PULSE_PRESS_MILLIS	(500)

typedef enum {
	PRESS_STATE_UP,
	PRESS_STATE_BOUNCE,
	PRESS_STATE_SHORT_PRESS,
	PRESS_STATE_LONG_PRESS,
	PRESS_STATE_SHORT_RELEASE,
	PRESS_STATE_LONG_RELEASE,
	PRESS_STATE_LONG_PULSE,
	PRESS_STATES_COUNT,
} press_state_t;

typedef void (*function_t)();

static void empty_function() {}
// #define empty_function NULL


class ButtonBitmask {
	public:
		~ButtonBitmask();
		ButtonBitmask();
		ButtonBitmask(function_t update_bitmask);
		void begin(function_t update_bitmask);
		void end();

		void update();
		void update(uint32_t cur_millis);
					
		bool is_up();
		bool is_down();
		bool is_pressed();
		bool is_released();
		bool is_pulsed();

		uint32_t get_up_millis();
		uint32_t get_down_millis();
		uint32_t get_pulse_count();
		press_state_t get_press_state();
		void set_next_pulse_millis(uint32_t next_pulse_millis);
		
		bool button_bit;

		function_t on_short_press_func = empty_function;
		function_t on_short_release_func = empty_function;
		function_t on_long_press_func = empty_function;
		function_t on_long_release_func = empty_function;
		function_t on_long_pulse_func = empty_function;

		uint32_t short_press_millis = DEFAULT_SHORT_PRESS_MILLIS;
		uint32_t long_press_millis = DEFAULT_LONG_PRESS_MILLIS;
		uint32_t pulse_press_millis = DEFAULT_PULSE_PRESS_MILLIS;

	// friend:
		// void _update_bitmask();
	protected:
		virtual void __update_bitmask();
		virtual void _match_bitmasks();
		virtual void _press_up();
		virtual bool _is_new_bitmask();
		virtual bool _is_old_bitmask();
		virtual bool _is_bitmasks_match();

	private:
		function_t _update_bitmask = NULL;
		bool _button_bit;
		press_state_t _press_state;
		uint32_t _up_millis;
		uint32_t _down_millis;
		uint32_t _old_millis;
		uint32_t _pulse_count;
		uint32_t _next_pulse_millis;
};

class ButtonsBitmask : public ButtonBitmask {
	public:
		~ButtonsBitmask();
		ButtonsBitmask();
		ButtonsBitmask(function_t update_bitmask, uint8_t buttons_count);
		void begin(function_t update_bitmask, uint8_t buttons_count);
		void end();

		uint8_t get_buttons_count();
		uint8_t get_button_index();
		uint8_t * buttons_bitmask = NULL;

	protected:
		virtual void _press_up();
		virtual void _match_bitmasks();
		virtual bool _is_new_bitmask();
		virtual bool _is_old_bitmask();
		virtual bool _is_bitmasks_match();
		virtual uint8_t * _malloc_and_copy(uint8_t * src, uint8_t size);
		uint8_t * _buttons_bitmask = NULL;
		uint8_t _buttons_bitmask_size;
		uint8_t _buttons_count;
	// private:
		// void __update_bitmask();
};

class Key : public ButtonBitmask {
	public:
		Key();
		Key(uint8_t pin);
		void begin(uint8_t pin);

	private:
		void __update_bitmask();
		uint8_t _pin;
};

class Keys : public ButtonsBitmask {
	public:
		~Keys();
		Keys();
		Keys(uint8_t * pins, uint8_t pins_count);
		void begin(uint8_t * pins, uint8_t pins_count);
		void end();

	private:
		void __update_bitmask();
		uint8_t * _buttons_pins = NULL;
};

class Keypad : public ButtonsBitmask {
	public:
		~Keypad();
		Keypad();
		Keypad(uint8_t * rows_pins, uint8_t * columns_pins, uint8_t rows_count, uint8_t columns_count);
		void begin(uint8_t * rows_pins, uint8_t * columns_pins, uint8_t rows_count, uint8_t columns_count);
		void end();

	private:
		void __update_bitmask();
		uint8_t * _rows_pins = NULL;
		uint8_t * _columns_pins = NULL;
		uint8_t _rows_count;
		uint8_t _columns_count;
};

#endif /* BUTTONS_H */
