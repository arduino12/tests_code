#define FAIL_PIN		(2)
#define CLOCK_PIN		(3) // 16Khz max HIGH
#define START_PIN		(4)
#define BREAK_PIN		(5)	// Active on LOW
#define DIR_PIN			(6)
#define GAIN_PIN		(7)

#define JOYSTICK_PIN	(A4)

#define DELAY			(1000)

const int16_t SPEEDS[] = {-10, -20, -30, -50, -70, -100, -200, 0, 0, 200, 100, 70, 50, 30, 20, 10};

void stop() {
	digitalWrite(START_PIN, LOW);
	digitalWrite(BREAK_PIN, LOW);
}

void start() {
	// stop();
	// digitalWrite(GAIN_PIN, LOW);
	digitalWrite(START_PIN, HIGH);
	// delay(1);
	digitalWrite(START_PIN, LOW);
	
	// delay(10);
	digitalWrite(BREAK_PIN, HIGH);
	// digitalWrite(GAIN_PIN, HIGH);
}

void step(int16_t steps, uint16_t speed) {
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
}

void setup() {
	Serial.begin(115200);
	Serial.println("Start!");

	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(START_PIN, OUTPUT);
	pinMode(BREAK_PIN, OUTPUT);
	pinMode(DIR_PIN, OUTPUT);
	pinMode(13, OUTPUT);
	
	pinMode(JOYSTICK_PIN, INPUT);
	
	// pinMode(GAIN_PIN, OUTPUT);
	digitalWrite(BREAK_PIN, HIGH);
	digitalWrite(START_PIN, LOW);
	// digitalWrite(GAIN_PIN, HIGH);
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
			// if (cur_speed)	start();
			// else			stop();
			digitalWrite(BREAK_PIN, cur_speed != 0);
		}
	
		if (cur_speed) {
			if ((cur_ms - old_ms) > (uint32_t)cur_speed) {
				old_ms = cur_ms;
				old_clock = !old_clock;
				// start();
				digitalWrite(START_PIN, HIGH);
				digitalWrite(CLOCK_PIN, HIGH);
				delay(1);
				digitalWrite(START_PIN, LOW);
				digitalWrite(CLOCK_PIN, LOW);
				digitalWrite(13, old_clock);
			}
		}
		delay(1);
	}
}
