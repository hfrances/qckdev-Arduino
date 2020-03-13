/*
 Name:		qckdev.h
 Created:	3/2/2020 5:56:52 PM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#ifndef _qckdev_ESP_WebServer_h
#define _qckdev_ESP_WebServer_h

#include <String>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef ESP32
//#include <WiFi.h>
#include <WebServer.h>
#define WEBSERVERNAME_CLASSNAME WebServer
#define WEBSERVERRESPONSE_CLASSNAME WebServerResponse
#elif defined(ESP8266)
//#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define WEBSERVERNAME_CLASSNAME ESP8266WebServer
#define WEBSERVERRESPONSE_CLASSNAME ESP8266WebServerResponse
#else
#error Platform not supported
#endif

class WEBSERVERRESPONSE_CLASSNAME {
private:
	int code;
	const char* contentType;
	String content;

public:
	void set(int code, const char* contentType = (const char*)0, const String& content = ((String)(""))) {
		this->code = code;
		this->contentType = contentType;
		this->content = content;
	}

	void send(WEBSERVERNAME_CLASSNAME& server) {
		server.sendHeader("Access-Control-Allow-Origin", "*");
		server.send(this->code, this->contentType, this->content);
	}
};

typedef WEBSERVERNAME_CLASSNAME WebServer;
typedef WEBSERVERRESPONSE_CLASSNAME WebServerResponse;

#endif

