#include <Wire.h>				// for LiquidCrystal_I2C class
#include <LiquidCrystal_I2C.h>	// for i2c lcd display module
#include <MD_TCS230.h>
#include <FreqCount.h>

// Pin definitions
#define S2_OUT					(12)
#define S3_OUT					(13)
#define OE_OUT					(8)	// LOW = ENABLED 
#define LCD_SDA_PIN				(A4)
#define LCD_SCL_PIN				(A5)

#define LCD_I2C_ADDRESS			(0x27)
#define LCD_WIDTH				(16)
#define LCD_HEIGHT				(2)

MD_TCS230 CS(S2_OUT, S3_OUT, OE_OUT);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);


//{ helpers
void lcd_printf_P(uint8_t line_index, const char * format, ...) {
	char buffer[LCD_WIDTH + 1];
	
	/* call sprintf_P with the given va_list */
	va_list argp;
	va_start(argp, format);
	vsprintf_P(buffer, format, argp);
	va_end(argp);
	
	/* print the formated text to lcd */
	lcd.setCursor(0, line_index);
	lcd.print(buffer);
}

void readSensor()
{
	static  bool  waiting;

	if (!waiting) {
		CS.read();
		waiting = true;
	}
	else {
		if (CS.available()) {
			colorData  rgb;

			CS.getRGB(&rgb);
			CS.setFilter(TCS230_RGB_X);
			uint32_t v = CS.readSingle() / 1000;
			
			Serial.print("RGB [");
			Serial.print(rgb.value[TCS230_RGB_R]);
			Serial.print(",");
			Serial.print(rgb.value[TCS230_RGB_G]);
			Serial.print(",");
			Serial.print(rgb.value[TCS230_RGB_B]);
			Serial.println("]");
			lcd_printf_P(0, PSTR("R:%03d G:%03d"),
				rgb.value[TCS230_RGB_R],
				rgb.value[TCS230_RGB_G]);
			lcd_printf_P(1, PSTR("B:%03d A:%03d"),
				rgb.value[TCS230_RGB_B],
				v);
			waiting = false;
		}
	}
}

void setup() {
	Serial.begin(115200);

	/* initialize the lcd and its I2C expender */
	lcd.init(); 
	lcd.backlight();
	
	CS.begin();
}

void loop() {
	readSensor();
}
