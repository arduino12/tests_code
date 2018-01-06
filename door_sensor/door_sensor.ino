
#define TRIG_PIN		(12) // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN		(11) // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE	(200)

#define MAX_SENSOR_DISTANCE 500 // Maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back.
#define US_ROUNDTRIP_CM 57      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
#define NO_ECHO 0               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance.
#define MAX_SENSOR_DELAY 18000  // Maximum uS it takes for sensor to start the ping (SRF06 is the highest measured, just under 18ms).


uint8_t _triggerBit;
uint8_t _echoBit;
volatile uint8_t *_triggerOutput;
volatile uint8_t *_echoInput;
unsigned int _maxEchoTime;
unsigned long _max_time;


bool ping_trigger()
{
	*_triggerOutput &= ~_triggerBit; // Set the trigger pin low, should already be low, but this will make sure it is.
	delayMicroseconds(4); // Wait for pin to go low, testing shows it needs 4uS to work every time.
	*_triggerOutput |= _triggerBit;  // Set trigger pin high, this tells the sensor to send out a ping.
	delayMicroseconds(10); // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
	*_triggerOutput &= ~_triggerBit; // Set trigger pin back to low.

	_max_time =  micros() + MAX_SENSOR_DELAY; // Set a timeout for the ping to trigger.
	while (*_echoInput & _echoBit && micros() <= _max_time); // Wait for echo pin to clear.
	while (!(*_echoInput & _echoBit)) // Wait for ping to start.
		if (micros() > _max_time)
			return false; // Something went wrong, abort.

	_max_time = micros() + _maxEchoTime; // Ping started, set the timeout.
	return true; // Ping started successfully.
}

uint16_t ping()
{
	if (!ping_trigger()) return NO_ECHO; // Trigger a ping, if it returns false, return NO_ECHO to the calling function.
	while (*_echoInput & _echoBit) // Wait for the ping echo.
		if (micros() > _max_time) return NO_ECHO; // Stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.
	return (micros() - (_max_time - _maxEchoTime) - 5); // Calculate ping time, 5uS of overhead.
}

void setup()
{
	Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

	_triggerBit = digitalPinToBitMask(TRIG_PIN); // Get the port register bitmask for the trigger pin.
	_echoBit = digitalPinToBitMask(ECHO_PIN); // Get the port register bitmask for the echo pin.

	_triggerOutput = portOutputRegister(digitalPinToPort(TRIG_PIN)); // Get the output port register for the trigger pin.
	_echoInput = portInputRegister(digitalPinToPort(ECHO_PIN)); // Get the input port register for the echo pin.

	*portModeRegister(digitalPinToPort(TRIG_PIN)) |= _triggerBit;
	_maxEchoTime = min(MAX_DISTANCE, MAX_SENSOR_DISTANCE) * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2); // Calculate the maximum distance in uS.
}

void loop()
{
	uint16_t us = ping(); // Send ping, get ping time in microseconds (uS).
	Serial.print("Ping: ");
	Serial.print(us / US_ROUNDTRIP_CM);
	Serial.println("cm");
	delay(50);
}
