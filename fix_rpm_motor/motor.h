#ifndef MOTOR_H
#define MOTOR_H

#include "pwm_hack.h"
#include "log.h"

#define MOTOR_PWM_PIN			(10)
#define ENCODER_PIN				(3)

#define MIN_ENCODER_PULSE_MS	(500)
#define MIN_MOTOR_PWM			(110)
#define MAX_MOTOR_PWM			(250)
#define MOTOR_PWM_STEPS			(5)

uint8_t motor_current_pwm;
uint32_t motor_desired_encoder_pulse_ms;


void motor_set_pwm(uint8_t pwm) {
	if (motor_current_pwm == pwm)
		return;
	motor_current_pwm = pwm;
	analogWrite(MOTOR_PWM_PIN, pwm);
	LOG(F("motor_set_pwm: ")); LOGLN(pwm);
}

void motor_set_rpm(uint8_t rpm) {
	if (!rpm) {
		motor_set_pwm(0);
		motor_desired_encoder_pulse_ms = 0;
		return;
	}
	motor_desired_encoder_pulse_ms = 60000 / rpm;
	if (!motor_current_pwm)
		motor_set_pwm(MIN_MOTOR_PWM);
}

void motor_loop() {
	static uint32_t old_encoder_pulse_ms;
	uint32_t encoder_pulse_ms;

	if (!motor_desired_encoder_pulse_ms)
		return;

	if (digitalRead(ENCODER_PIN)) {
		encoder_pulse_ms = millis() - old_encoder_pulse_ms;
		if (encoder_pulse_ms < MIN_ENCODER_PULSE_MS)
			return;
		old_encoder_pulse_ms = millis();
		LOG(F("encoder_pulse_ms: ")); LOGLN(encoder_pulse_ms);
		LOG(F("motor_desired_encoder_pulse_ms: ")); LOGLN(motor_desired_encoder_pulse_ms);
		
		if (encoder_pulse_ms > motor_desired_encoder_pulse_ms)
			motor_set_pwm(min(MAX_MOTOR_PWM, motor_current_pwm + MOTOR_PWM_STEPS));
		else if (encoder_pulse_ms < motor_desired_encoder_pulse_ms)
			motor_set_pwm(max(MIN_MOTOR_PWM, motor_current_pwm - MOTOR_PWM_STEPS));
	}
}

void motor_setup() {
	setPwmFrequency(MOTOR_PWM_PIN, 128);
	pinMode(ENCODER_PIN, INPUT_PULLUP);
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	motor_set_rpm(0);
}

#endif /* MOTOR_H */
