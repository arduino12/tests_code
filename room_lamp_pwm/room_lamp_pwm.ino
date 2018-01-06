#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
 
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


void setup() {
	Serial.begin(115200);
	Serial.println("16 channel PWM test!");
	pwm.begin();
	pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

	// save I2C bitrate
	uint8_t twbrbackup = TWBR;
	// must be changed after calling Wire.begin() (inside pwm.begin())
	TWBR = 12; // upgrade to 400KHz!
}

void temp(int v) {
	pwm.setPWM(1, 0, 4095-v);
	Serial.println(v);
	delay(1);
}

void loop() {
//  if (Serial.available() > 0)
//    temp(Serial.parseInt());
//  temp(900, 1000);
//  temp(1000, 1000);
//  temp(2000, 1000);
//  temp(3000, 1000);
//  temp(4095, 1000);
	// Drive each PWM in a 'wave'
	for (uint16_t i=3000; i<4096; i ++) {
		temp(i);
	}
	for (uint16_t i=4096; i>3000; i --) {
		temp(i);
	}
}
