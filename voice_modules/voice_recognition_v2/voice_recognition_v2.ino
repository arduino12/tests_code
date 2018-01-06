/*********************************** LICENCE **********************************\
| Copyright (c) 2013, A.E. TEC                                                 |
| All rights reserved.                                                         |
|                                                                              |
| Redistribution and use in source and binary forms, with or without           |
| modification, are permitted provided that the following conditions are met:  |
|                                                                              |
| * Redistributions of source code must retain the above copyright notice,     |
|   this list of conditions and the following disclaimer.                      |
| * Redistributions in binary form must reproduce the above copyright notice,  |
|   this list of conditions and the following disclaimer in the documentation  |
|   and/or other materials provided with the distribution.                     |
|                                                                              |
| THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  |
| AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    |
| IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   |
| ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE    |
| LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR          |
| CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         |
| SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS     |
| INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      |
| CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)      |
| ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   |
| POSSIBILITY OF SUCH DAMAGE.                                                  |
\******************************************************************************/
/************************************ USES ************************************\
| This sketch demonstrates the use of Voice Recognition Module V2.             |
|                                                                              |
| It works with Uno/Leonardo, with Buttons/Encoder, and 16x2 LCD display       |
|               with or without "IIC I2C TWI SPI Serial Interface Board Module"|
|				(that saves your arduino pins when works with LCD)             |
|                                                                              |
| With this sketch, you can easily run and teach the V2 Voice Recognition      |
| 					Module with very simple interface via the LCD.             |
|                                                                              |
\******************************************************************************/
/*********************************** CIRCUIT **********************************\
| Parallel LCD:                                                                |
|	* LCD RS to pin 12                                                         |
|	* LCD E  to pin 11                                                         |
|	* LCD D4 to pin 5                                                          |
|	* LCD D5 to pin 4                                                          |
|	* LCD D6 to pin 3                                                          |
|	* LCD D7 to pin 2                                                          |
|	* LCD RW to GND                                                            |
| I2C LCD:                                                                     |
|	* LCD SDA pin to Leonardo pin 2 OR Uno pin A4                              |
|	* LCD SCL pin to Leonardo pin 3 OR Uno pin A5                              |
|                                                                              |
| 3 Buttons:                                                                   |
|	* button LEFT  between GND and pin 6                                       |
|	* button RIGHT between GND and pin 7                                       |
|	* button ENTER between GND and pin 8                                       |
| Rotary Encoder:                                                              |
|	* phase A to pin 6                                                         |
|	* phase B to pin 7                                                         |
|	* enter   to pin 8                                                         |
|	* com to GND                                                               |
|                                                                              |
| VR = Voice Recognition Module V2                                             |
|                                                                              |
| Uno:                                                                         |
|	* Connect VR.TX to pin 0 AFTER YOU BURN THIS SOFTWARE!                     |
|	* Connect VR.RX to pin 1 AFTER YOU BURN THIS SOFTWARE!                     |
	* after you connected RX & TX - press RESET button!                        |
| Leonardo:                                                                    |
|	* Connect VR.RX to pin 0                                                   |
|	* Connect VR.TX to pin 1                                                   |
\******************************************************************************/


// If you use parallel LCD- comment the following line (//#define USE_I2C_LCD)
#define USE_I2C_LCD

// If you use 3 simple buttons- comment the following line
//#define USE_ROTARY_ENCODER

// If you use standard arduino uno- comment the following line
//#define USE_ARDUINO_LEONARDO


//{ Includes

#if defined(USE_I2C_LCD)
	#include <Wire.h> 
	#include <LiquidCrystal_I2C.h>	
#else
	#include <LiquidCrystal.h>
#endif

//}

//{ Defines 

#if defined(USE_ARDUINO_LEONARDO)
	#define VR_BEGIN(bps)      Serial1.begin(bps)
	#define VR_WRITE(buf, len) Serial1.write(buf, len)
	#define VR_READ            Serial1.read()
	#define VR_AVAILABLE       Serial1.available()
	#define VR_FLUSH           Serial1.flush()
	#define VR_EVENT           serialEvent1
#else
	#define VR_BEGIN(bps)      Serial.begin(bps)
	#define VR_WRITE(buf, len) Serial.write(buf, len)
	#define VR_READ            Serial.read()
	#define VR_AVAILABLE       Serial.available()
	#define VR_FLUSH           Serial.flush()
	#define VR_EVENT           serialEvent
#endif

const char C_MAIN[] PROGMEM = "Main";



const char C_RETURN    [] PROGMEM = "Return";
const char C_GROUP     [] PROGMEM = "Group_";
const char C_GROUPS    [] PROGMEM = "Groups";
const char C_OUTPUTS   [] PROGMEM = "Outputs";
const char C_SETTINGS  [] PROGMEM = "Settings";

const char C_LISTEN    [] PROGMEM = "Listen";
const char C_LERN      [] PROGMEM = "Learn";
const char C_DELETE    [] PROGMEM = "Delete";

const char C_RESET     [] PROGMEM = "Reset";
const char C_MODE      [] PROGMEM = "Mode";
const char C_DUTY      [] PROGMEM = "Duty";

const char C_BAUD_RATE [] PROGMEM = "Baun Rate";
const char C_INFO      [] PROGMEM = "Info";
const char C_RESTORE   [] PROGMEM = "Restore";

const char C_PULSE     [] PROGMEM = "Pulse";
const char C_FLIP      [] PROGMEM = "Flip";
const char C_DOWN      [] PROGMEM = "Down";
const char C_UP        [] PROGMEM = "Up";

const char C_STATUS    [] PROGMEM = "Status";
const char C_FINISH    [] PROGMEM = "Finish";
const char C_YES       [] PROGMEM = "Yes";
const char C_NO        [] PROGMEM = "No";

const char C_OK        [] PROGMEM = "OK";
const char C_ERROR     [] PROGMEM = "ERROR";
const char C_FAILED    [] PROGMEM = "FAILED";
const char C_TIMEOUT   [] PROGMEM = "TIMEOUT";

const char C_NO_REC    [] PROGMEM = "NO REC";
const char C_1_REC     [] PROGMEM = "1 REC";
const char C_2_REC     [] PROGMEM = "2 REC";
const char C_1_2_REC   [] PROGMEM = "1,2 REC";
const char C_3_REC     [] PROGMEM = "3 REC";
const char C_1_3_REC   [] PROGMEM = "1,3 REC";
const char C_2_3_REC   [] PROGMEM = "2,3 REC";
const char C_ALL_REC   [] PROGMEM = "ALL REC";

const char C_SPEAK     [] PROGMEM = "SPEAK";
const char C_TOO_QUIET [] PROGMEM = "TOO QUIETE";
const char C_AGAIN     [] PROGMEM = "AGAIN";
const char C_TOO_LOUD  [] PROGMEM = "TOO LOUD";
const char C_DIFFERENT [] PROGMEM = "DIFFERENT";
const char C_GOOD      [] PROGMEM = "GOOD";
const char C_FINISH_1  [] PROGMEM = "FINISH 1";
const char C_FINISH_2  [] PROGMEM = "FINISH 2";
const char C_FINISH_3  [] PROGMEM = "FINISH 3";
const char C_ClearLine [] PROGMEM = "                ";


#define M_MAIN      P(C_MAIN)
#define M_RETURN    P(C_RETURN)
#define M_GROUP     P(C_GROUP)
#define M_GROUPS    P(C_GROUPS)
#define M_OUTPUTS   P(C_OUTPUTS)
#define M_SETTINGS  P(C_SETTINGS)

#define M_LISTEN    P(C_LISTEN)
#define M_LERN      P(C_LERN)
#define M_DELETE    P(C_DELETE)

#define M_RESET     P(C_RESET)
#define M_MODE      P(C_MODE)
#define M_DUTY      P(C_DUTY)

#define M_BAUD_RATE P(C_BAUD_RATE)
#define M_INFO      P(C_INFO)
#define M_RESTORE   P(C_RESTORE)

#define M_PULSE     P(C_PULSE)
#define M_FLIP      P(C_FLIP)
#define M_DOWN      P(C_DOWN)
#define M_UP        P(C_UP)

#define M_STATUS    P(C_STATUS)
#define M_FINISH    P(C_FINISH)
#define M_YES       P(C_YES)
#define M_NO        P(C_NO)

#define M_OK        P(C_OK)
#define M_ERROR     P(C_ERROR)
#define M_FAILED    P(C_FAILED)
#define M_TIMEOUT   P(C_TIMEOUT)

#define M_NO_REC    P(C_NO_REC)
#define M_1_REC     P(C_1_REC)
#define M_2_REC     P(C_2_REC)
#define M_1_2_REC   P(C_1_2_REC)
#define M_3_REC     P(C_3_REC)
#define M_1_3_REC   P(C_1_3_REC)
#define M_2_3_REC   P(C_2_3_REC)
#define M_ALL_REC   P(C_ALL_REC)

#define M_SPEAK     P(C_SPEAK)
#define M_TOO_QUIET P(C_TOO_QUIET)
#define M_AGAIN     P(C_AGAIN)
#define M_TOO_LOUD  P(C_TOO_LOUD)
#define M_DIFFERENT P(C_DIFFERENT)
#define M_GOOD      P(C_GOOD)
#define M_FINISH_1  P(C_FINISH_1)
#define M_FINISH_2  P(C_FINISH_2)
#define M_FINISH_3  P(C_FINISH_3)

//} 

//{ Consts

const byte ENC_A_PIN = 6;
const byte ENC_B_PIN = 7;
const byte ENC_E_PIN = 8;

const byte ENC_LEFT  = 1;
const byte ENC_RIGHT = 2;
const byte ENC_ENTER = 4;

const byte MENU_MAIN              = 0;
	const byte MENU_GROUP1        = 1;
	const byte MENU_GROUP2        = 2;
	const byte MENU_GROUP3        = 3;
	const byte MENU_GROUPS        = 4;
		const byte MENU_LISTEN    = 5;
		const byte MENU_LERN      = 6;
		const byte MENU_DELETE    = 7;
	const byte MENU_OUTPUTS       = 8;
		const byte MENU_RESET     = 9;
		const byte MENU_MODE      = 10;
		const byte MENU_DUTY 	  = 11;
	const byte MENU_SETTINGS      = 12;
		const byte MENU_BAUD_RATE = 13;
		const byte MENU_INFO      = 14;
		const byte MENU_RESTORE   = 15;

const uint16_t VR_CMD_TIMEOUT = 1000;
const uint16_t VR_BAUD        = 9600;

const byte VR_HEAD       = 0xAA;
const byte VR_WAITING    = 0x00;
const byte VR_GROUP_1    = 0x01;
const byte VR_GROUP_2    = 0x02;
const byte VR_GROUP_3    = 0x03;
const byte VR_GROUP_ALL  = 0x04;

const byte VR_DELETE     = 0x00;
const byte VR_DELETE_1   = VR_DELETE + VR_GROUP_1;
const byte VR_DELETE_2   = VR_DELETE + VR_GROUP_2;
const byte VR_DELETE_3   = VR_DELETE + VR_GROUP_3;
const byte VR_DELETE_ALL = VR_DELETE + VR_GROUP_ALL;
const byte VR_LEARN      = 0x10;
const byte VR_LEARN_1    = VR_LEARN + VR_GROUP_1;
const byte VR_LEARN_2    = VR_LEARN + VR_GROUP_2;
const byte VR_LEARN_3    = VR_LEARN + VR_GROUP_3;
const byte VR_IMPORT     = 0x20;
const byte VR_IMPORT_1   = VR_IMPORT + VR_GROUP_1;
const byte VR_IMPORT_2   = VR_IMPORT + VR_GROUP_2;
const byte VR_IMPORT_3   = VR_IMPORT + VR_GROUP_3;
const byte VR_IMPORTED   = VR_IMPORT + VR_GROUP_ALL;
const byte VR_FINISH     = 0x45;
const byte VR_FINISH_1   = VR_FINISH + VR_GROUP_1;
const byte VR_FINISH_2   = VR_FINISH + VR_GROUP_2;
const byte VR_FINISH_3   = VR_FINISH + VR_GROUP_3;

const byte VR_COMMON     = 0x36;
const byte VR_COMPACT    = 0x37;

const byte VR_OK         = 0xcc;
const byte VR_ERROR      = 0xe0;
const byte VR_FAILED     = 0xe1;
const byte VR_TIMEOUT    = 0xe2;

const byte VR_SPEAK      = 0x40;
const byte VR_TOO_QUIETE = 0x41;
const byte VR_AGAIN      = 0x42;
const byte VR_TOO_LOUD   = 0x43;
const byte VR_DIFFERENT  = 0x44;
const byte VR_FINISH_ONE = 0x45;

const byte VR_BAUD_RATE  = 0x30;
const byte VR_RESET_O    = 0x40;
const byte VR_RESET_O1   = 0x41;
const byte VR_RESET_O2   = 0x42;
const byte VR_RESET_O3   = 0x43;
const byte VR_RESET_O4   = 0x44;
const byte VR_RESET_O5   = 0x45;
const byte VR_RESET_ALL  = 0x46;
const byte VR_MODE       = 0x4F;
const byte VR_MODE_PULSE = 0x50;
const byte VR_MODE_FLIP  = 0x51;
const byte VR_MODE_DOWN  = 0x52;
const byte VR_MODE_UP    = 0x53;

const byte VR_DUTY       = 0x5F;
const byte VR_RESTORE    = 0x70;
const byte VR_VERSION    = 0xBB;

//}

//{ Veraibles

#if defined(USE_I2C_LCD)
	LiquidCrystal_I2C lcd(0x27, 16, 2);
#else
	LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#endif

byte old_enc;
byte cur_menu, sel_max, sel_index;
byte cur_group, cur_Rec, vrRespond;

String s_menu[18];

//}

//{ Main and Loop

void setup()
{
	//Serial.begin(9600);
	VR_BEGIN(VR_BAUD);

	
#if defined(USE_I2C_LCD)
	lcd.init();
    lcd.backlight();
#else
	lcd.begin(16, 2);
#endif
    
	pinMode(ENC_E_PIN, INPUT_PULLUP);
	pinMode(ENC_A_PIN, INPUT_PULLUP);
	pinMode(ENC_B_PIN, INPUT_PULLUP);
	
	sendCommandVR(VR_COMPACT);
	sendCommandVR(VR_WAITING);

	setMenu(MENU_MAIN);
}

void loop()
{
	byte cur_enc = 0;
	if (!digitalRead(ENC_A_PIN)) cur_enc |= ENC_LEFT;
	if (!digitalRead(ENC_B_PIN)) cur_enc |= ENC_RIGHT;
	if (!digitalRead(ENC_E_PIN)) cur_enc |= ENC_ENTER;
	if (old_enc == cur_enc) return;
#if defined(USE_ROTARY_ENCODER)	
	if (cur_enc == 0)
	{
		if      (old_enc == ENC_LEFT)  keyRight();
		else if (old_enc == ENC_RIGHT) keyLeft();
	}
	else if (cur_enc == ENC_ENTER) keyEnter();
#else
	if      (cur_enc == ENC_LEFT)  keyLeft();
	else if (cur_enc == ENC_RIGHT) keyRight();
	else if (cur_enc == ENC_ENTER) keyEnter();
#endif
	old_enc = cur_enc;
	delay(10);
}

//}

//{ Keys Events

void keyLeft()
{
	if (--sel_index > sel_max) sel_index = sel_max;
	refSell();
}

void keyRight()
{
	if (++sel_index > sel_max) sel_index = 0;
	refSell();
}

void keyEnter()
{
	if (cur_menu == MENU_MAIN)
	{
		if      (sel_index == 0) setMenu(MENU_GROUP1);
		else if (sel_index == 1) setMenu(MENU_GROUP2);
		else if (sel_index == 2) setMenu(MENU_GROUP3);
		else if (sel_index == 3) setMenu(MENU_GROUPS);
		else if (sel_index == 4) setMenu(MENU_OUTPUTS);
		else                     setMenu(MENU_SETTINGS); // (sel_index == 5)
	}
	else if (cur_menu < MENU_GROUPS)
	{
		if      (sel_index == 0) setMenu(MENU_MAIN);
		else if (sel_index == 1) setMenu(MENU_LISTEN);
		else if (sel_index == 2) setMenu(MENU_LERN);
		else                     setMenu(MENU_DELETE); // (sel_index == 3)
	}
	else if (cur_menu == MENU_GROUPS)
	{
		if      (sel_index == 0) setMenu(MENU_MAIN);
		else if (sel_index == 1) showMsg((String)M_STATUS + ": " + getStatusVR());
		else                     setMenu(MENU_DELETE); // (sel_index == 2)
	}
	else if (cur_menu == MENU_OUTPUTS)
	{
		if      (sel_index == 0) setMenu(MENU_MAIN);
		else if (sel_index == 1) setMenu(MENU_RESET);
		else if (sel_index == 2) setMenu(MENU_MODE);
		else                     setMenu(MENU_DUTY); // (sel_index == 3)
	}
	else if (cur_menu == MENU_SETTINGS)
	{
		if      (sel_index == 0) setMenu(MENU_MAIN);
		else if (sel_index == 1) setMenu(MENU_BAUD_RATE);
		else if (sel_index == 2) setMenu(MENU_INFO);
		else                     setMenu(MENU_RESTORE); // (sel_index == 3)
	}
	else if (cur_menu == MENU_LISTEN) // (sel_index == 0)
	{
		sendCommandVR(VR_WAITING);
		setMenu(cur_group);
	}
	else if (cur_menu == MENU_LERN) // (sel_index == 0)
	{
		if (vrRespond == VR_FINISH + cur_group) setMenu(cur_group);
	}
	else if (cur_menu == MENU_DELETE)
	{
		if      (sel_index == 0) setMenu(cur_group);
		else	showMsg(getCmdTextVR(VR_DELETE + cur_group));
	}
	else if (cur_menu == MENU_RESET)
	{
		if      (sel_index == 0) setMenu(MENU_OUTPUTS);
		else	showMsg(getCmdTextVR(VR_RESET_O + sel_index));
	}
	else if (cur_menu == MENU_MODE)
	{
		if      (sel_index == 0) setMenu(MENU_OUTPUTS);
		else	showMsg(getCmdTextVR(VR_MODE + sel_index));
	}
	else if (cur_menu == MENU_DUTY)
	{
		if      (sel_index == 0) setMenu(MENU_OUTPUTS);
		else	showMsg(getCmdTextVR(VR_DUTY + sel_index));
	}
	else if (cur_menu == MENU_BAUD_RATE)
	{
		if      (sel_index == 0) setMenu(MENU_SETTINGS);
		else
		{
			uint16_t tmp = (12 << sel_index) * 100;
			showMsg(getCmdTextVR(VR_BAUD_RATE + sel_index));
			delay(100);
			VR_BEGIN(tmp);
			showMsg((String)tmp + " bps");
		}
	}
	else if (cur_menu == MENU_INFO) // (sel_index == 0)
	{
		if      (sel_index == 0) setMenu(MENU_SETTINGS);
		else
		{
			sendCommandVR(VR_COMMON);
			delay(10);
			sendCommandVR(VR_VERSION);		
			lcd.setCursor(0, 0);
			clearLine();
			lcd.setCursor(0, 1);
			clearLine();
			lcd.setCursor(0, 0);
			sel_index = sel_max = 0;
			for (uint16_t i = VR_CMD_TIMEOUT, b = 0; i; i--)
			{
				if (VR_AVAILABLE)
				{
					lcd.print((char)VR_READ);
					if (b++ == 16) lcd.setCursor(0, 1);
				}
				delay(1);
			}
			VR_FLUSH;
			sendCommandVR(VR_COMPACT);
		}
	}
	else if (cur_menu == MENU_RESTORE)
	{
		if      (sel_index == 0) setMenu(MENU_SETTINGS);
		else
		{
			showMsg(getCmdTextVR(VR_RESTORE));
			delay(100);
			VR_BEGIN(VR_BAUD);
		}
	}
}

//}

//{ Menus

void setMenu(byte new_menu)
{
	sel_index = 0;
	cur_menu = new_menu;
	if      (cur_menu == MENU_MAIN)
	{
		setMenu(M_MAIN, M_GROUP + 1, M_GROUP + 2, M_GROUP + 3, M_GROUPS, M_OUTPUTS, M_SETTINGS);
	}
	else if (cur_menu < MENU_GROUPS)
	{
		cur_group = cur_menu;
		setMenu(M_GROUP + cur_menu, M_RETURN, M_LISTEN, M_LERN, M_DELETE);
	}
	else if (cur_menu == MENU_GROUPS)
	{
		cur_group = 4;
		setMenu(M_GROUPS, M_RETURN, M_STATUS, M_DELETE);
	}
	else if (cur_menu == MENU_OUTPUTS)
	{
		setMenu(M_OUTPUTS, M_RETURN, M_RESET, M_MODE, M_DUTY);
	}
	else if (cur_menu == MENU_SETTINGS)
	{
		setMenu(M_SETTINGS, M_RETURN, M_BAUD_RATE, M_INFO, M_RESTORE);
	}
	else if (cur_menu == MENU_LISTEN)
	{
		setMenu(M_LISTEN + " " + cur_group + ": " + getCmdTextVR(VR_IMPORT + cur_group), M_RETURN);
	}
	else if (cur_menu == MENU_LERN)
	{
		cur_Rec = 1;
		sendCommandVR(VR_LEARN + cur_group);
		setMenu(M_LERN + " " + cur_group + ": ", M_RETURN);
	}
	else if (cur_menu == MENU_DELETE)
	{
		setMenu((cur_group < 4 ? addGroup(M_DELETE) : (String)M_DELETE + " ALL") + " ?", M_NO, M_YES);
	}
	else if (cur_menu == MENU_RESET)
	{
		printTitle(M_RESET);
		s_menu[0] = M_RETURN;
		for (byte i = 1; i < 6; i++)
			s_menu[i] = "O" + (String)i;
		s_menu[6] = "ALL";
		sel_max = 6;
	}
	else if (cur_menu == MENU_MODE)
	{
		setMenu(M_MODE, M_RETURN, M_PULSE, M_FLIP, M_DOWN, M_UP);
	}
	else if (cur_menu == MENU_DUTY)
	{
		printTitle(M_DUTY);
		s_menu[0] = M_RETURN;
		int t = 10;
		for (byte i = 1; i < 17; i++)
		{
			s_menu[i] = (String)(t) + " ms";
			if      (i < 5)   t += 5;
			else if (i == 5)  t = 50;
			else if (i < 11)  t += 10;
			else if (i < 15)  t += 100;
			else              t = 1000;
		}
		sel_max = 16;
	}
	else if (cur_menu == MENU_BAUD_RATE)
	{
		printTitle(M_BAUD_RATE);
		s_menu[0] = M_RETURN;
		for (byte i = 1; i < 6; i++)
			s_menu[i] = (String)(12 << i) + "00 bps";
		sel_max = 5;
	}
	else if (cur_menu == MENU_INFO)
	{
		setMenu(M_INFO, M_RETURN, M_INFO);
	}
	else if (cur_menu == MENU_RESTORE)
	{
		setMenu(M_RESTORE + " ?", M_NO, M_YES);
	}
	refSell();
}

void showMsg(String s)
{
	sel_index = 0;
	setMenu(s, M_RETURN);
	refSell();
}

void printTitle(String s)
{
	lcd.setCursor(0, 0);
	delay(10);
	clearLine();
	delay(10);
	lcd.setCursor(0, 0);
	delay(10);
	lcd.print(s);
	delay(10);
}

void printMenu(String s)
{
	lcd.setCursor(0, 1);
	delay(10);
	clearLine();
	delay(10);
	lcd.setCursor(1, 1);
	delay(10);
	lcd.print(s);
	delay(10);
}

void clearLine()
{
	lcd.print(P(C_ClearLine));
}

void refSell()
{
	printMenu(s_menu[sel_index]);
}

void setMenu(String title, String m0)
{
	printTitle(title);
	sel_max = 0;
	s_menu[0] = m0;
}
void setMenu(String title, String m0, String m1)
{
	printTitle(title);
	sel_max = 1;
	s_menu[0] = m0;
	s_menu[1] = m1;
}
void setMenu(String title, String m0, String m1, String m2)
{
	printTitle(title);
	sel_max = 2;
	s_menu[0] = m0;
	s_menu[1] = m1;
	s_menu[2] = m2;
}
void setMenu(String title, String m0, String m1, String m2, String m3)
{
	printTitle(title);
	sel_max = 3;
	s_menu[0] = m0;
	s_menu[1] = m1;
	s_menu[2] = m2;
	s_menu[3] = m3;
}
void setMenu(String title, String m0, String m1, String m2, String m3, String m4)
{
	printTitle(title);
	sel_max = 4;
	s_menu[0] = m0;
	s_menu[1] = m1;
	s_menu[2] = m2;
	s_menu[3] = m3;
	s_menu[4] = m4;
}
void setMenu(String title, String m0, String m1, String m2, String m3, String m4, String m5)
{
	printTitle(title);
	sel_max = 5;
	s_menu[0] = m0;
	s_menu[1] = m1;
	s_menu[2] = m2;
	s_menu[3] = m3;
	s_menu[4] = m4;
	s_menu[5] = m5;
}

//}

//{ Others

byte sendCommandVR(byte cmd)
{
	vrRespond = VR_TIMEOUT;
	VR_WRITE(((byte[]){VR_HEAD, cmd}), 2);
	
	for (uint16_t i = VR_CMD_TIMEOUT; i && (vrRespond == VR_TIMEOUT); i--)
	{
		if (VR_AVAILABLE) vrRespond = VR_READ;
		delay(1);
	}
	return vrRespond;
}

String getCmdTextVR(byte cmd)
{
	sendCommandVR(cmd);
	if (vrRespond == VR_OK)      return M_OK;
	if (vrRespond == VR_ERROR)   return M_ERROR;
	if (vrRespond == VR_FAILED)  return M_FAILED;
	if (vrRespond == VR_TIMEOUT) return M_TIMEOUT;
	return (String)vrRespond;
}

String getStatusVR()
{
	sendCommandVR(VR_IMPORTED);
	if (vrRespond == 0)        return M_NO_REC;
	if (vrRespond == 1)        return M_1_REC;
	if (vrRespond == 2)        return M_2_REC;
	if (vrRespond == 3)        return M_1_2_REC;
	if (vrRespond == 4)        return M_3_REC;
	if (vrRespond == 5)        return M_1_3_REC;
	if (vrRespond == 6)        return M_2_3_REC;
	if (vrRespond == 7)        return M_ALL_REC;
	if (vrRespond == VR_ERROR) return M_ERROR;
	return (String)vrRespond;
}

String getInstructionTextVR()
{
	if (vrRespond == VR_ERROR)      return M_ERROR;
	if (vrRespond == VR_TIMEOUT)    return M_TIMEOUT;
	if (vrRespond == VR_SPEAK)      return M_SPEAK;
	if (vrRespond == VR_TOO_QUIETE) return M_TOO_QUIET;
	if (vrRespond == VR_AGAIN)      return M_AGAIN;
	if (vrRespond == VR_TOO_LOUD)   return M_TOO_LOUD;
	if (vrRespond == VR_DIFFERENT)  return M_DIFFERENT;
	if (vrRespond == VR_FINISH_ONE) return M_GOOD;
	if (vrRespond == VR_FINISH_1)   return M_FINISH_1;
	if (vrRespond == VR_FINISH_2)   return M_FINISH_2;
	if (vrRespond == VR_FINISH_3)   return M_FINISH_3;
	return (String)vrRespond;
}

String addGroup(String s)
{
	return s + " " + (String)M_GROUP + cur_group;
}

char getGroupChar()
{
	return ('@' + cur_group);
}

void VR_EVENT()
{
	if (VR_AVAILABLE) vrRespond = VR_READ;
	if      (cur_menu == MENU_LISTEN)
	{
		printMenu(getGroupChar() + (String)(vrRespond - 0x10 * cur_group));
	}
	else if (cur_menu == MENU_LERN)
	{
		if (vrRespond == VR_FINISH + cur_group)
		{
			sel_max = sel_index = 0;
			printTitle(addGroup(M_FINISH) + "!");
			refSell();
			return;
		}
		if (vrRespond == VR_FINISH_ONE) cur_Rec++;
		printMenu(getGroupChar() + (String)cur_Rec + ": " + getInstructionTextVR());
	}
}
/*
String P(PGM_P s)
{
	char c;
	String tmp = "";
	while ((c = pgm_read_byte(s++)))
		tmp += c;
	return tmp;
}
*/
String P(const prog_char str[])
{
	char c;
	String tmp = "";
	if(str)
		while ((c = pgm_read_byte(str++)))
			tmp += c;
	return tmp;
}


//}




