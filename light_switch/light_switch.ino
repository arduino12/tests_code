#include <Servo.h>
#include <IRremote.h>

#define IR_RECIVER_PIN				(10)
#define SERVO_SWITCH_PIN			(11)
#define IR_LONG_PRESS_KEY_ID		(0xFFFFFFFF)
#define SERVO_SWITCH_ON_DEG			(60)
#define SERVO_SWITCH_OFF_DEG		(120)
#define SERVO_SWITCH_MIDDLE_DEG		(95)

/* servo object to control our servo */
Servo switch_servo;
/* ir reciver object to read remote controle buttons */
IRrecv irrecv(IR_RECIVER_PIN);
decode_results results;
/* current switch state */
bool is_switch_on;


/* called when IR remote button pressed */
void ir_recived(uint32_t id) {
	switch (id) {
		/* remote - button */
		case 0xFFE01F:
			turn_switch(false);
		break;
		/* remote + button */
		case 0xFFA857:
			turn_switch(true);
		break;
		/* remote EQ button */
		case 0xFF906F:
			turn_switch(!is_switch_on);
		break;
		default:
			Serial.print(F("unknown ir key: "));
			Serial.println(id, HEX);
		break;
	}
}

void turn_switch(bool is_on) {
	Serial.print(F("turn switch: "));
	Serial.println(is_on ? "on" : "off");
	is_switch_on = is_on;
	switch_servo.write(is_on ? SERVO_SWITCH_ON_DEG : SERVO_SWITCH_OFF_DEG);
	delay(400);
	switch_servo.write(SERVO_SWITCH_MIDDLE_DEG);
	delay(400);
}

void delay_and_recive(uint32_t ms) {
	ms += millis();
	while (ms > millis()) {
		if (irrecv.decode(&results)) {
			if (results.value != IR_LONG_PRESS_KEY_ID) {
				ir_recived(results.value);
			}
			/* receive ir next value */
			irrecv.resume();
		}
	}
}

/* called once at power-on */
void setup() {
	/* start serial uart with 115200 baudrate */
	Serial.begin(115200);
	Serial.println(F("start!"));
	/* attaches the servo object to its GPIO pin */
	switch_servo.attach(SERVO_SWITCH_PIN);
	turn_switch(false);
	/* start ir receiver */
	irrecv.enableIRIn();
}

/* called repeatedly after "setup" */
void loop() {
	// turn_switch(true);
	// delay_and_recive(1000);
	// turn_switch(false);
	delay_and_recive(1000);
}
