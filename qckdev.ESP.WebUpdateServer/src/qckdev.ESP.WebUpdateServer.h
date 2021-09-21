#pragma once

#ifndef _qckdev_ESP_WebUpdateServer_h
#define _qckdev_ESP_WebUpdateServer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <qckdev.ESP.WebServer.h>
#include <qckdev.ESP.Updater.h>

#ifdef ESP32
#define WEBUPDATESERVERNAME_CLASSNAME WebUpdateServer
#elif defined(ESP8266)
#define WEBUPDATESERVERNAME_CLASSNAME ESP8266WebUpdateServer
#else
#error Platform not supported
#endif

class WEBUPDATESERVERNAME_CLASSNAME {

private:
	bool _serial_output;
	WEBSERVERNAME_CLASSNAME *_server;
	String _username;
	String _password;
	bool _authenticated;
	String _updaterError;

public:
	WEBUPDATESERVERNAME_CLASSNAME(bool serial_debug = false);
	void setup(WEBSERVERNAME_CLASSNAME *server);
	void setup(WEBSERVERNAME_CLASSNAME *server, const String& path);
	void setup(WEBSERVERNAME_CLASSNAME *server, const String& username, const String& password);
	void setup(WEBSERVERNAME_CLASSNAME *server, const String& path, const String& username, const String& password);
	void updateCredentials(const String& username, const String& password);

};

typedef WEBUPDATESERVERNAME_CLASSNAME WebUpdateServer;

#endif

