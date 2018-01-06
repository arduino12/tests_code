#ifndef PCA9685_H
#define PCA9685_H

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void pca9685_write(uint8_t index, uint16_t value) {
	pwm.setPWM(index, 0, value);
}

void pca9685_setup() {
	pwm.begin();
	/* change after calling Wire.begin() (inside pwm.begin()) for 400KHz */
	TWBR = 12;
	/* maximum PWM frequency*/
	pwm.setPWMFreq(1600);
}

#endif /* PCA9685_H */
