/*
 * Read parallel flash chip using Arduino-Mega via serial.
 * Chip: 27C128 Obsolete Microchip 128K (16K x 8) CMOS EPROM.
 * Arad Eizen 04/08/18.
 */
#include <serial_menu.h>

/*
 * Connections:
 * / ---------------------- \
 * | 27C128  | Arduino-Mega	|
 * | ------- + ------------ |
 * | VCC     | 5V           |
 * | VSS     | GND          |
 * | CE, OE  | GND          |
 * | PGM     | 5V           |
 * | A0-A13  | A0-A13       |
 * | O0-O7   | 22-29        |
 * \ ---------------------- /
 */

/* uart baud rate in bit per seconds */
#define SERIAL_BAUD_RATE			(115200)
/* set uart read timeout in milliseconds */
#define SERIAL_TIMEOUT				(1)


/* prints serial menu commands help */
void print_help()
{
	Serial.println(F(
		"______________________\n"
		"\n"
		"  Read Parallel Flash:\n"
		"______________________\n"
		"\n"
		"H\n"
		"    Print this help\n"
		"R <read(address[0-16383] size[1-64])>\n"
		"    Read <size> bytes from <address>\n"
		"W <write(address[0-16383] data[[1-64]])>\n"
		"    Write data bytes starting at <address>\n"
	));
}

void read_to_serial(uint16_t address, uint16_t size)
{
	uint8_t value;
	while(size--) {
		PORTF = address & 0xFF;
		PORTK = address >> 8;
		address++;
		delayMicroseconds(1);
		value = PINA;
		if (value < 16)
			Serial.print(0);
		Serial.print(value, HEX);
	}
	PORTF = PORTK = 0;
	Serial.println();
}

void setup()
{
	/* initialize uart for serial communication */
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.setTimeout(SERIAL_TIMEOUT);

	/* set pins A0-A13 as OUTPUT LOW */
	DDRF = 0xFF;
	DDRK |= 0x3F;
	PORTF = PORTK = 0;

	/* set pins 22-29 as INPUTS */
	DDRA = 0;
	PORTA = 0;

	/* print commands to terminal */
	print_help();
}

void loop()
{
	uint16_t a = 0, b = 0;

	if (serial_menu_update()) {
		switch (command.buffer[0]) {
			case 'H':
				print_help();
				break;
			case 'R':
				b = 1;
				sscanf(command.buffer + 2, "%d %d", &a, &b);
				Serial.print(F("read("));
				Serial.print(a);
				Serial.print(F(", "));
				Serial.print(b);
				Serial.println(')');
				read_to_serial(a, b);
				break;
		}
		return;
	}
}
