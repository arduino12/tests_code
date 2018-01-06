#ifndef LOG_H
#define LOG_H

#define UART_LOG

#ifdef UART_LOG
	#define LOG_INIT			Serial.begin(115200)
	#define LOG(...)			Serial.print(__VA_ARGS__)
	#define LOGLN(...)			Serial.println(__VA_ARGS__)
	#define LOG_P(log, ...)		Serial.print(F(log), __VA_ARGS__)
	#define LOGLN_P(log, ...)	Serial.println(F(log), __VA_ARGS__)
#else
	#define LOG_INIT
	#define LOG(...)
	#define LOGLN(...)
#endif /* UART_LOG */

#endif /* LOG_H */
