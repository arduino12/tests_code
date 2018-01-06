#define STEP_PIN		(A0)
#define ENABLE_PIN		(A1)
#define DIR_PIN			(A2)

#define JOYSTICK_PIN	(A4)

#define DELAY			(1000)

const int16_t SPEEDS[] = {-1, -2, -5, -10, 0, 10, 2, 5, 1};


/*void step(int16_t steps, uint16_t speed) {
	digitalWrite(DIR_PIN, steps < 0);
	if (steps < 0) steps = -steps;

	if (!steps) {
		stop();
		return;
	}

	start();
	while (steps--) {
		digitalWrite(CLOCK_PIN, HIGH);
		delay(speed);
		digitalWrite(CLOCK_PIN, LOW);
		delay(speed);
	}
	stop();
}*/

void setup() {
	Serial.begin(115200);
	Serial.println("Start!");

	pinMode(13, OUTPUT);
	pinMode(STEP_PIN, OUTPUT);
	pinMode(DIR_PIN, OUTPUT);
	pinMode(ENABLE_PIN, OUTPUT);
	pinMode(JOYSTICK_PIN, INPUT);
	
	digitalWrite(ENABLE_PIN, HIGH);
}

void loop() {
	uint32_t old_ms = 0;
	uint32_t cur_ms;
	int16_t cur_speed = 0;
	uint8_t old_speed_index = 0;
	uint8_t cur_speed_index = 0;
	bool old_clock = false;
	// start();
	while (true) {
		cur_ms = millis();
		cur_speed_index = map(analogRead(JOYSTICK_PIN), 0, 1023, 0, sizeof(SPEEDS) / sizeof(*SPEEDS) - 1);
		// Serial.println(cur_speed_index);
		if (old_speed_index != cur_speed_index) {
			old_speed_index = cur_speed_index;
			cur_speed = SPEEDS[cur_speed_index];
			digitalWrite(DIR_PIN, cur_speed > 0);
			Serial.println(cur_speed);
			cur_speed = abs(cur_speed);
			digitalWrite(ENABLE_PIN, cur_speed == 0);
			digitalWrite(13, cur_speed != 0);
		}
	
		if (cur_speed) {
			if ((cur_ms - old_ms) > (uint32_t)cur_speed) {
				old_ms = cur_ms;
				old_clock = !old_clock;
				digitalWrite(STEP_PIN, old_clock);
				digitalWrite(13, old_clock);
			}
		}
		// delay(1);
	}
}
