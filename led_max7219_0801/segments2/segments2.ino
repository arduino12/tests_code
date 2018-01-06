/*

MAX7219 ON 8 * 7 Segment Screen:

/=====================\
|   8 8 8 8 8 8 8 8   |
| ------------------- |
| | OO | RR | GG | G| |
\=====================/
      | | | | | |
      1 2 3 4 5 6
	  
Pinout:
	1 = VCC (+5V)
	2 = GND
	3 = CLK
	4 = DATA
	5 = LOAD
	6 = DOUT
*/

const int CLK_PIN  = A0;
const int DATA_PIN = A1;
const int LOAD_PIN = A2;

const int CHIP_COUNT = 1;

String inputString = "12345678";
boolean stringComplete = true;

void setup() 
{
	pinMode(CLK_PIN, OUTPUT);
	pinMode(LOAD_PIN, OUTPUT);
	pinMode(DATA_PIN, OUTPUT);

	Serial.begin(115200);
	inputString.reserve(200);
	
	Init();
	

}

void loop()
{
	for (int i = 1; i < 9; i++)
     	for (int j = 2; j < 511; j <<= 1)
		{
			SentData(i, j-1);
			delay(50);
		}
	for (int i = 1; i < 9; i++)
     	for (int j = 511; j > 0; j >>= 1)
		{
			SentData(i, j-1);
			delay(50);
		}
  /*
	if (stringComplete)
	{
		Serial.println(inputString);
		char tmp = inputString.charAt(0);
		inputString = inputString.substring(1); // inputString.length()
		switch (tmp)
		{
			case 'D':
         
			break;

			case 'M':
				SentData(inputString.charAt(0),inputString.charAt(1));
			break;
			case 'T':
				for (int i = 1; i < 9; i++)
					SentData(i, 255);
			break;
			
			//default:
			
			//break;
		}

		inputString = "";
		stringComplete = false;
		//delay(100);
		}*/
}

void serialEvent()
{
	while (Serial.available())
	{
		char inChar = (char)Serial.read(); 
		if (inChar == '\n') stringComplete = true;
		else inputString += inChar;
	}
}



void Init()
{
	digitalWrite(CLK_PIN, LOW);
	digitalWrite(LOAD_PIN, LOW);
	digitalWrite(DATA_PIN, LOW);
	SentData(11, 7);    // Max Scan Limit
	SentData(9, 0);     // Matrix Decode Mode
	SetMatrixIntensity(15);   // Medium Intensity (1-15)

	for (int i = 1; i < 9; i++)
		SentData(i, 0); //All Red And Green Leds = Off

	SetMatrixPower(true);     //Shutdown Mode
}


void SetMatrixPower(bool value)
{
	SentData(12, value ? 1 : 0);
}

void SetMatrixIntensity(int value)
{
	SentData(10, value);
}
/*
void SentData(int addres, int[] Data)
{
	for (int i=0; i<CHIP_COUNT;i++)
		SentChipData(addres, Data[i]);
	OnOff(LOAD_PIN);
}
*/

void SentDataN(int addres, int Data)
{
	for (int i = 0; i < CHIP_COUNT; i++)
		SentChipData(addres, Data);
	OnOff (LOAD_PIN);
}

void SentData(int addres, int Data)
{
	SentChipData(addres, Data);
	OnOff (LOAD_PIN);
}

void SentChipData(int addres, int value)
{
	for (int i = 0; i < 4; i++)
		OutputChang(false, DATA_PIN);             // Dont care

	for (int i = 8; i > 0; i /= 2)
		OutputChang((addres & i) != 0, DATA_PIN); // Adresse

	for (int i = 128; i > 0; i /= 2)
		OutputChang((value & i) != 0, DATA_PIN);  // Data
}

void OutputChang(bool value, int bit)
{
	digitalWrite(bit, value);
	if (bit == DATA_PIN) OnOff (CLK_PIN);    // Clook_Puls after Data sent
}

void OnOff(int bit)
{
	OutputChang(true, bit);
	OutputChang(false, bit);
}











