#include "interactive_menu.h"
#include "pca9685.h"

/* uart baud rate in bit per seconds */
#define SERIAL_BAUD_RATE	(115200)
/* set uart read timeout in milliseconds */
#define SERIAL_TIMEOUT		(1)

void setup() {
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.setTimeout(SERIAL_TIMEOUT);
	pca9685_setup();
	update_color();
	print_help();
}

void loop() {
	interactive_menu_update();
}
