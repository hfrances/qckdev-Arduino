/*
 Name:		qckdev.h
 Created:	3/3/2020 11:40:53 AM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#ifndef _qckdev_ESP_WiFi_h
#define _qckdev_ESP_WiFi_h

#include <String>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef ESP32
#include <WiFi.h>
typedef WiFiMode_t WiFiMode;
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error Platform not supported
#endif

extern WiFiMode StringToWiFiMode(const char* name);

extern const char* WiFiModeToString(const WiFiMode value);

#endif

