#define LOAD	(2)
#define CP		(3)
#define DF		(4)
#define DI		(5)


void setup() {
	Serial.begin(115200);
	pinMode(LOAD, OUTPUT);
	pinMode(CP, OUTPUT);
	pinMode(DF, OUTPUT);
	pinMode(DI, OUTPUT);

	digitalWrite(LOAD, LOW);
	digitalWrite(CP, LOW);
	digitalWrite(DF, HIGH);
	digitalWrite(DI, LOW);
}

void loop() {
	static uint8_t b = 0;
	for (uint8_t i = 0; i < 40; i++) {
		digitalWrite(DI, (i % 1) != b);
		digitalWrite(CP, HIGH);
		digitalWrite(CP, LOW);
	}
	digitalWrite(LOAD, HIGH);
	digitalWrite(LOAD, LOW);
	b = !b;
	delay(5000);
}
