/*********************************** CIRCUIT **********************************\
| Parallel LCD 128x64 Connections:                                             |
|   /-------------------------------\                                          |
|   | Pin Assignment: (See Notes)   |                                          |
|   |---------------+---------------|                                          |
|   | LCD:          | UNO:          |                                          |
|   |---------------+---------------|                                          |
|   | PIN:  | NAME: | PIN:  | NAME: |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 1     | VSS   |       | GND   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 2     | VCC   |       | +5V   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 3     | V0    |       |       |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 4     | RS    |       |       |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 5     | RW    |       | GND   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 6     | EN    | 2     | PD2   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 7     | DB0   | A0    | PC0   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 8     | DB1   | A1    | PC1   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 9     | DB2   | A2    | PC2   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 10    | DB3   | A3    | PC3   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 11    | DB4   | 4     | PD4   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 12    | DB5   | 5     | PD5   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 13    | DB6   | 6     | PD6   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 14    | DB7   | 7     | PD7   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 15    | CS1   | 8     | PB0   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 16    | CS2   | 9     | PB1   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 17    | RST   |       | +5V   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 18    | VOUT  |       |       |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 19    | LEDA  |       | +5V   |                                          |
|   |-------+-------+-------+-------|                                          |
|   | 20    | LEDK  |       |       |                                          |
|   \-------+-------+-------+-------/                                          |
| Notes:                                                                       |
|   *               10K                                                        |
|   * LCD PIN 18 -/\/\/\/- GND                                                 |
|   *                ^                                                         |
|   *                |                                                         |
|   *            LCD PIN 3                                                     |
|   *               _____                                                      |
|   * LCD PIN 8 >--|     \                                                     |
|   *              | NAND )o--> LCD PIN 4                                      |
|   * LCD PIN 9 >--|_____/                                                     |
|   *               _____                                                      |
|   *              |     \                                                     |
|   * UNO PIN 3 >--| NOT  )o--> LCD PIN 20                                     |
|   *              |_____/                                                     |
|                                                                              |
\******************************************************************************/

// un-comment only ONE of the following definitions:
#define DISPLAY_128X64  // 1 LCD Screen
//#define DISPLAY_256X64  // 2 LCD Screens
//#define DISPLAY_128X128 // 2 LCD Screens
//#define DISPLAY_384X64  // 3 LCD Screens
//#define DISPLAY_128X192 // 3 LCD Screens
//#define DISPLAY_512X64  // 4 LCD Screens
//#define DISPLAY_128X256 // 4 LCD Screens
//#define DISPLAY_256X128 // 4 LCD Screens

#define LCD_CS1_MASK            (0x01) // Arduino Pin 8
#define LCD_CS2_MASK            (0x02) // Arduino Pin 9
#define LCD_CS3_MASK            (0x04) // Arduino Pin 10
#define LCD_CS4_MASK            (0x08) // Arduino Pin 11
#define LCD_CS5_MASK            (0x10) // Arduino Pin 12
#define LCD_CS6_MASK            (0x20) // Arduino Pin 13
#define LCD_CS7_MASK            (0x40) // Arduino Pin A4
#define LCD_CS8_MASK            (0x80) // Arduino Pin A5

#if defined(DISPLAY_128X64)
    #define LCD_COUNT           (1)
    #define DISPLAY_WIDTH       (128)
    #define DISPLAY_HEIGHT      (64)
#elif defined(DISPLAY_256X64)
    #define LCD_COUNT           (2)
    #define DISPLAY_WIDTH       (256)
    #define DISPLAY_HEIGHT      (64)
#elif defined(DISPLAY_128X128)
    #define LCD_COUNT           (2)
    #define DISPLAY_WIDTH       (128)
    #define DISPLAY_HEIGHT      (128)
#elif defined(DISPLAY_384X64)
    #define LCD_COUNT           (3)
    #define DISPLAY_WIDTH       (384)
    #define DISPLAY_HEIGHT      (64)
#elif defined(DISPLAY_128X192)
    #define LCD_COUNT           (3)
    #define DISPLAY_WIDTH       (128)
    #define DISPLAY_HEIGHT      (192)
#elif defined(DISPLAY_512X64)
    #define LCD_COUNT           (4)
    #define DISPLAY_WIDTH       (512)
    #define DISPLAY_HEIGHT      (64)
#elif defined(DISPLAY_128X256)
    #define LCD_COUNT           (4)
    #define DISPLAY_WIDTH       (128)
    #define DISPLAY_HEIGHT      (256)
#elif defined(DISPLAY_256X128)
    #define LCD_COUNT           (4)
    #define DISPLAY_WIDTH       (256)
    #define DISPLAY_HEIGHT      (128)
#endif

#if LCD_COUNT == 1
    #define LCD_CS_ALL_MASK     (LCD_CS1_MASK | LCD_CS2_MASK)
#elif LCD_COUNT == 2
    #define LCD_CS_ALL_MASK     (LCD_CS1_MASK | LCD_CS2_MASK | \
                                 LCD_CS3_MASK | LCD_CS4_MASK)
#elif LCD_COUNT == 3
    #define LCD_CS_ALL_MASK     (LCD_CS1_MASK | LCD_CS2_MASK | \
                                 LCD_CS3_MASK | LCD_CS4_MASK | \
                                 LCD_CS5_MASK | LCD_CS6_MASK)
#elif LCD_COUNT == 4
    #define LCD_CS_ALL_MASK     (LCD_CS1_MASK | LCD_CS2_MASK | \
                                 LCD_CS3_MASK | LCD_CS4_MASK | \
                                 LCD_CS5_MASK | LCD_CS6_MASK | \
                                 LCD_CS7_MASK | LCD_CS8_MASK)
#endif

#define LCD_EN_MASK             (0x04)
                               
#define UART_BAUD_2000000       (0x00) // Not stable enough...
#define UART_BAUD_1000000       (0x01)
#define UART_BAUD_500000        (0x03)
#define UART_BAUD_250000        (0x07)
#define UART_BAUD_125000        (0x0F)
                               
#define LCD_WIDTH               (64)
#define LCD_HEIGHT              (64)
#define LCD_LINES               (8)






void setup() {
	DDRD |= 0xFC;                 // Arduino Pins 2 - 7 OUTPUTS
	DDRC |= 0x0F;                 // Arduino Pins A0 - A3 OUTPUTS
	DDRB |= 0x3F;                 // Arduino Pins 8 - 13 OUTPUTS

	lcd_init();                   // Initialize LCD(s)
	lcd_brightness(160);          // Set LCD(s) Back-light Brightness
	uart_init(UART_BAUD_1000000); // Initialize Arduino UART

	for (uint8_t y = 0; y < LCD_LINES; y++) {
		lcd_write(0xB8 | y, LCD_CS_ALL_MASK);
		for (uint8_t x = 0, m = 1; x < LCD_WIDTH; x++, m <<= 1) {
			if (0 == m) m = 1;
			lcd_write(m, LCD_CS1_MASK);
			lcd_write(~m, LCD_CS2_MASK);
#if LCD_COUNT >= 2
			lcd_write(m, LCD_CS3_MASK);
			lcd_write(~m, LCD_CS4_MASK);
#endif
#if LCD_COUNT >= 3
			lcd_write(m, LCD_CS5_MASK);
			lcd_write(~m, LCD_CS6_MASK);
#endif
#if LCD_COUNT == 4
			lcd_write(m, LCD_CS7_MASK);
			lcd_write(~m, LCD_CS8_MASK);
#endif
		}
	}

	while(1)
		for (uint8_t y = 0; y < LCD_LINES; y++) {
			lcd_write(0xB8 | y, LCD_CS_ALL_MASK);
			for (uint8_t x = 0; x < LCD_WIDTH; x++) {
				lcd_write(uart_read(), LCD_CS1_MASK);
				lcd_write(uart_read(), LCD_CS2_MASK);
#if LCD_COUNT >= 2
				lcd_write(uart_read(), LCD_CS3_MASK);
				lcd_write(uart_read(), LCD_CS4_MASK);
#endif
#if LCD_COUNT >= 3
				lcd_write(uart_read(), LCD_CS5_MASK);
				lcd_write(uart_read(), LCD_CS6_MASK);
#endif
#if LCD_COUNT == 4
				lcd_write(uart_read(), LCD_CS7_MASK);
				lcd_write(uart_read(), LCD_CS8_MASK);
#endif
			}
		}
}

void loop() { }

void lcd_init() {
	lcd_write(0x3E, LCD_CS_ALL_MASK); // Off
	lcd_write(0x3F, LCD_CS_ALL_MASK); // On
	lcd_write(0xC0, LCD_CS_ALL_MASK); // Display Start Line
	lcd_write(0xB8, LCD_CS_ALL_MASK); // Y = 0
	lcd_write(0x40, LCD_CS_ALL_MASK); // X = 0
}

void lcd_write(uint8_t value, uint8_t chip_select) {
	PORTD &= 0x0B;
#if LCD_COUNT == 4
    PORTC &= 0xC0;
#else
	PORTC &= 0xF0;
#endif
	PORTB &= ~LCD_CS_ALL_MASK;
	PORTB |= chip_select;
#if LCD_COUNT == 4
    PORTC |= value & 0x0F | (chip_select >> 2) & 0x30;
#else
	PORTC |= value & 0x0F;
#endif
	PORTD |= value & 0xF0 | LCD_EN_MASK;
	delayMicroseconds(1);
	PORTD ^= LCD_EN_MASK;
	delayMicroseconds(6);
}

void lcd_brightness(uint8_t brightness) {
	analogWrite(3, brightness);
}

void uart_init(uint8_t uart_baud) {
	cli();                               // Disable Global Interrupts
	UCSR0A |= _BV(U2X0);                 // Double Transmission Speed
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);   // Enable Transmitter And Receiver
	UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01); // 8 data bits, 1 stop bit
	UBRR0H = 0;                          // Baud Rate For F_CPU = 16MHz:
	UBRR0L = uart_baud;                  // Fix Bauds: 2Mb, 1Mb, 0.5Mb...
	sei();                               // Enable global interrupts
}

volatile uint8_t uart_read(){
    while(!(UCSR0A & _BV(RXC0)));
    return UDR0;
}

volatile void uart_write(uint8_t value){
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = value;
}
