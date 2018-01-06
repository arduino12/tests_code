/*
 MyMP3 - Library to control a WTV020-SD-16P module to play voices from an Arduino board.
 Created by: Arad Eizen 13/6/13
 */

#include <MyMP3.h>

const int RST_PIN = 2;  // The pin number of the reset pin.
const int CLK_PIN = 3;  // The pin number of the clock pin.
const int DAT_PIN = 4;  // The pin number of the data pin.
const int BUS_PIN = 5;  // The pin number of the busy pin.

MyMP3 myMP3(RST_PIN,CLK_PIN,DAT_PIN,BUS_PIN);

int tmp = 7;

void setup()
{
	myMP3.reset();  //Initializes the module.

	myMP3.volume(7); 
	myMP3.play(1);
	delay(5000);
	myMP3.play(2);
	delay(5000);
	myMP3.play(3);
	//myMP3.pause();
	//myMP3.stop();
	//delay(2000);

	//delay(2000);
}

void loop()
{
//myMP3.volume(tmp);
/*
if (tmp<7) tmp++;
else tmp = 0;
delay(2000);
*/
 /*
  myMP3.play(1);
  delay(5000);
  myMP3.pause();
  delay(5000);
  myMP3.pause();
  delay(5000);  
  myMP3.stop();
  myMP3.play(3);
  delay(2000);   
  myMP3.volume(1);
  delay(2000);
  myMP3.volume(6);
  delay(2000);    
  myMP3.stop();
  */
}


