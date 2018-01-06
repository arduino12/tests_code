#include <MyMP3.h>

const int RST_PIN = 4;
const int CLK_PIN = 5;
const int DAT_PIN = 6;
const int BUS_PIN = 7;

MyMP3 myMP3(RST_PIN,CLK_PIN,DAT_PIN,BUS_PIN);

void setup()
{
	myMP3.reset();  //Initializes the module.
	/*
	myMP3.playAll(1000);
	myMP3.playAll(1001);
	myMP3.playAll(1002);
	myMP3.playAll(1003);
	myMP3.playAll(1004);
	myMP3.playAll(1005);
	myMP3.playAll(1006);
	myMP3.playAll(1007);
	myMP3.playAll(1008);
	myMP3.playAll(1009);
	*/
	
	delay(1000);
	
	myMP3.volume(5);
	
	//playDay(0);
	//playDay(1);
	//myMP3.play(1);
	//delay(5000);
	//myMP3.pause();
	//myMP3.stop();
	//delay(2000);
}

void loop()
{
	for (int i = 0; i < 101; i++)
		playNum(i);
}

void playNum(int num)
{
	myMP3.playAll(102 + num);
	delay(50);
}

void playDay(int num)
{
	myMP3.playAll(1400 + num);
	delay(50);
}