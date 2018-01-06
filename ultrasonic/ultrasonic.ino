// #include <Ultrasonic.h>

#define A_TRIG_PIN			(2)
#define A_ECHO_PIN			(3)
#define B_TRIG_PIN			(4)
#define B_ECHO_PIN			(5)
#define MAX_ECHO_SET_US		(1000)
#define MAX_ECHO_CLEAR_US	(3200)
#define REPEATED_COUNT		(2)
// #define MAX_DISTANCE	(200)
// #define MAX_DISTANCE_DELAY	(11400)
// Ultrasonic sonar_a(A_TRIG_PIN, A_ECHO_PIN, MAX_DISTANCE);
// Ultrasonic sonar_b(B_TRIG_PIN, B_ECHO_PIN, MAX_DISTANCE);


uint8_t ping() {
	uint8_t ret = 3;
	uint32_t timeout;

	// send a ping
	PORTD |= 0x14;
	delayMicroseconds(10);
	PORTD &= ~0x14;

	// wait for echo to rise- ping sent
	timeout = micros() + MAX_ECHO_SET_US;
	while ((PIND & 0x28) && (micros() < timeout));
	while ((PIND & 0x28) != 0x28) if (micros() > timeout) return 0;

	// wait for echo to fall- ping recived
	timeout = micros() + MAX_ECHO_CLEAR_US;
	while ((PIND & 0x28) == 0x28) if (micros() > timeout) return 0;

	return (PIND & 0x08 ? 1 : 2);
/*
	// wait for echo to fall- ping recived
	delayMicroseconds(MAX_ECHO_CLEAR_US);
	if (PIND & 0x08) ret -= 1;
	if (PIND & 0x20) ret -= 2;

	return ret;*/
}

void setup() {
	Serial.begin(115200);
		
	DDRD |= 0x14;				// (2, 4) OUTPUTS
	PORTD &= ~0x14;				// (2, 4) LOW
}

void loop() {
	static uint8_t state = 0;
	static uint8_t old_ping;
	static uint8_t first_ping;
	static uint8_t last_ping;
	uint8_t current_ping;
	uint8_t current_dir = 0;
	
	static uint8_t stable_a = 0, stable_b = 0;
	static uint8_t repeated_a = 0, repeated_b = 0;
	uint8_t current_a, current_b;
	
	current_ping = ping();

	switch (state) {
		case 0:
			if (current_ping) {
				first_ping = last_ping = current_ping;
				state = 1;
				// Serial.println("A");
			}
		break;
		default:
			if (current_ping) {
				last_ping = current_ping;
				// Serial.println("B");
				state = 1;
			}
			else if (++state > 7) {
				// Serial.println("C");
				if (first_ping != last_ping)
					current_dir = first_ping;
				state = 0;
			}
		break;
	}
	
	
	stable_a = current_a = current_ping & 1;
	stable_b = current_b = current_ping >> 1;
	
	/*
	if (current_a) {
		repeated_a = REPEATED_COUNT;
		stable_a = current_a;
	}
	else {
		if (!repeated_a) {
			stable_a = current_a;
		}
		else repeated_a--;
	}
	
	if (current_b) {
		repeated_b = REPEATED_COUNT;
		stable_b = current_b;
	}
	else {
		if (!repeated_b) {
			stable_b = current_b;
		}
		else repeated_b--;
	}
	*/
	Serial.print(stable_a);
	Serial.print(' ');
	Serial.print(stable_b);
	Serial.print(' ');
	Serial.println(current_dir + 2);
	
	
	/*
	if (stable_a != current_a) {
		if (!repeated_a) {
			repeated_a = REPEATED_COUNT;
			stable_a = current_a;
		}
		else repeated_a--;
	}
	else if (repeated_a < REPEATED_COUNT) repeated_a++;
	
	if (stable_b != current_b) {
		if (!repeated_b) {
			repeated_b = REPEATED_COUNT;
			stable_b = current_b;
		}
		else repeated_b--;
	}
	else if (repeated_b < REPEATED_COUNT) repeated_b++;
	*/
	
	
	/*
	static uint8_t a_samples;
	static uint8_t b_samples;
	static uint8_t sample_index = 1;
	
	
	if (sample_index < (1 << (SAMPLE_COUNT - 1)))
		sample_index <<= 1;
	else
		sample_index = 1;
	
	a_samples = current_ping & 1;
	b_samples = current_ping >> 1;
	for (uint8_t i)
	*/
	// Serial.print("A: ");
	// Serial.print(sonar_a.ping() / US_ROUNDTRIP_CM);
	// Serial.print(", B: ");
	// Serial.println(sonar_a.ping() / US_ROUNDTRIP_CM);

	// Serial.print(sonar_a.ping_median());
	// Serial.print(' ');
	// Serial.println(sonar_b.ping_median());
	
	// Serial.println(PIND & 0x28);

	/*
	if (old_ping != current_ping) {
		old_ping = current_ping;
		if (current_ping && old_dir != current_ping) {
			old_dir = current_ping;
			Serial.println(current_ping);
		}
	}
	*/
	

	
	delay(10);
}
