#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
 
#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA (1<<PB3)           //MOSI (SI)
#define LATCH (1<<PB2)          //SS   (RCK)
#define CLOCK (1<<PB5)          //SCK  (SCK)
 
#define COLUMN_REGISTER DDRD
#define COLUMN_PORT PORTD
#define COLUMN0 (1<<PD3)
#define COLUMN1 (1<<PD4)
#define COLUMN2 (1<<PD5)
#define COLUMN3 (1<<PD6)
#define COLUMN4 (1<<PD7)
 
volatile uint8_t bitplane = 0, column = 0, counter = 0;
 
volatile uint8_t framebuffer[] = { 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
 
};
 
void hdsp2011_put_pixel(int8_t x, int8_t y, int8_t color) {
        int8_t moddiv5 [] = { 
                0x00, 0x10, 0x20, 0x30, 0x40, 
                0x01, 0x11, 0x21, 0x31, 0x41, 
                0x02, 0x12, 0x22, 0x32, 0x42, 
                0x03, 0x13, 0x23, 0x33, 0x43, 
                0x04, 0x14, 0x24, 0x34, 0x44, 
                0x05, 0x15, 0x25, 0x35, 0x45, 
                0x06, 0x16, 0x26, 0x36, 0x46, 
                0x07, 0x17, 0x27, 0x37, 0x47
        };
 
        //int16_t pixnum = (x/5)*7 + (x%5)*56 + y;
        // Is the following faster than the line above?
        int16_t pixnum = (moddiv5[x] & 0x0F)*7 + (moddiv5[x]>>4)*56 + y;
 
        int16_t addr = pixnum >> 3;
        int8_t bit = pixnum & 0x07;
        // Clear the pixel
        framebuffer[addr] &= ~(1 << bit);
        framebuffer[addr+35] &= ~(1 << bit);
        // Draw new pixel
        if (color & 1) framebuffer[addr] |= (1 << bit);
        if (color & 2) framebuffer[addr+35] |= (1 << bit);
 
}
volatile int8_t x = 0, y = 0, dx = 1, dy = 1, color = 2; 
volatile uint8_t bnc_cntr = 0;

void frame_complete() {
        // Bouncing Pixel!
        if ((counter == 10)) {
                hdsp2011_put_pixel(x, y, (color-2) % 4);
                x += dx;
                y += dy;
                 
                if (x < 0) {
                        x = 0;
                        //goto out_x;   // Use goto if you *need* to save a few bytes of flash
                        dx = -dx;
                        bnc_cntr++;
                }
                if(x > 39) { 
                        x = 39;
//out_x:
                        dx = -dx;
                        bnc_cntr++;
                }
                if (y < 0) {
                        y = 0;
                        //goto out_y;
                        dy = -dy;
                        bnc_cntr++;
                }
                if (y > 6) {
                        y = 6;
//out_y:
                        dy = -dy;
                        bnc_cntr++;
                }
                hdsp2011_put_pixel(x, y, 3);
                counter = 0;
                if (bnc_cntr > 64) { 
                        color++;
                        bnc_cntr = 0;
                }
        }
}
 
ISR(TIMER2_OVF_vect) {
        int8_t i;
        // Blank the display
        COLUMN_PORT &= ~(COLUMN0 | COLUMN1 | COLUMN2 | COLUMN3 | COLUMN4);
 
        for(i=6; i>=0; i--) {
                // There was simpler way, it looks better (on the display) this way though:
                if (bitplane == 0) {
                        SPDR = framebuffer[column*7 + i];
                } else {
                        if (bitplane < 4) {
                                SPDR = framebuffer[35 + column*7 + i];
                        } else {
                                SPDR = (framebuffer[column*7 + i] & framebuffer[35 + column*7 + i]);
                        }
                }
 
                /*
                switch (bitplane) {
                        case 0:
                                SPDR = framebuffer[column*7 + i];
                                break;
                        case 1:
                        case 2:
                        case 3:
                                SPDR = framebuffer[35+column*7 + i];
                                break;
                        default:
                                SPDR = (framebuffer[column*7 + i] & framebuffer[35+column*7 + i]);
                }
                */
                while(!(SPSR & (1<<SPIF)));
        }
        // *** upside-down
        //COLUMN_PORT |= (1 << (7-column));     // Select proper column (un-blank) 
        // *** normal
        COLUMN_PORT |= (8 << column);   // Select proper column (un-blank)
        bitplane++;
        if (bitplane > 6) {
                bitplane = 0;
                column++;
        }
 
        // Once every screen refresh:
        if (column == 5) {
                counter++;
                frame_complete(); // This could run in a main loop, keeping ISR short, but...
                column = 0;
        }
		// TCNT1 = 0;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);
  
  // Setup IO
  SHIFT_REGISTER |= (DATA | LATCH | CLOCK);     // Set control pins as outputs
  SHIFT_PORT &= ~(DATA | LATCH | CLOCK);                // Set control pins low
 
  // Setup SPI
  // SPI enable, master, rising SCK trailing edge
  // *** upside-down
  //SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL) | (1<<DORD);
  // *** normal
  SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL);
 
  // Setup column port
  // Columns at bits 0 to 4 as outputs
  COLUMN_REGISTER |= COLUMN0 | COLUMN1 | COLUMN2 | COLUMN3 | COLUMN4;
  // Drive columns blank
  COLUMN_PORT &= ~(COLUMN0 | COLUMN1 | COLUMN2 | COLUMN3 | COLUMN4);
 
  // Pull LATCH low (Important: this is necessary to start the SPI transfer!)
  SHIFT_PORT &= ~LATCH;
/*
  // Timer prescaler: 
  // 001 1
  // 010 /8
  // 011 /64
  // 100 /256
  // 101 /1024
  // 110 ext. rising edge
  // 111 ext. falling edge
  TCCR0 |= (1<<CS01);// | (1<<CS00);
  //TCCR0 |= (1<<CS02);// | (1<<CS00);
  // Enable timer overflow interrupt
  TIMSK |= (1<<TOIE0);
  // Initialize timer
  TCNT0 = 0;
*/
  // Enable interrupts
  // sei();
  /*
  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2B |= (1 << CS20);    // 256 prescaler 
  TIMSK2 |= (1 << TOIE0);   // enable timer overflow interrupt
  TCNT2 = 0;            // preload timer 65536-16MHz/256/2Hz
  interrupts();             // enable all interrupts
  */
}

// the loop function runs over and over again forever
void loop() {
  // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(1000);                       // wait for a second
	// sleep_enable();
	//sei();       
	// sleep_cpu();
	// sleep_disable();
	        int8_t i;
        // Blank the display
        COLUMN_PORT &= ~(COLUMN0 | COLUMN1 | COLUMN2 | COLUMN3 | COLUMN4);
 
        for(i=6; i>=0; i--) {
                // There was simpler way, it looks better (on the display) this way though:
                if (bitplane == 0) {
                        SPDR = framebuffer[column*7 + i];
                } else {
                        if (bitplane < 4) {
                                SPDR = framebuffer[35 + column*7 + i];
                        } else {
                                SPDR = (framebuffer[column*7 + i] & framebuffer[35 + column*7 + i]);
                        }
                }
 
                /*
                switch (bitplane) {
                        case 0:
                                SPDR = framebuffer[column*7 + i];
                                break;
                        case 1:
                        case 2:
                        case 3:
                                SPDR = framebuffer[35+column*7 + i];
                                break;
                        default:
                                SPDR = (framebuffer[column*7 + i] & framebuffer[35+column*7 + i]);
                }
                */
                while(!(SPSR & (1<<SPIF)));
        }
        // *** upside-down
        //COLUMN_PORT |= (1 << (7-column));     // Select proper column (un-blank) 
        // *** normal
        COLUMN_PORT |= (8 << column);   // Select proper column (un-blank)
        bitplane++;
        if (bitplane > 6) {
                bitplane = 0;
                column++;
        }
 
        // Once every screen refresh:
        if (column == 5) {
                counter++;
                frame_complete(); // This could run in a main loop, keeping ISR short, but...
                column = 0;
        }
}
