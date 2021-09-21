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
#define WEBSERVERNAME_NAMESPACE webserver
#define WEBSERVERNAME_CLASSNAME WebServer
#define WEBSERVERRESPONSE_CLASSNAME WebServerResponse
#elif defined(ESP8266)
//#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define WEBSERVERNAME_NAMESPACE esp8266webserver
#define WEBSERVERNAME_CLASSNAME ESP8266WebServer
#define WEBSERVERRESPONSE_CLASSNAME ESP8266WebServerResponse
#else
#error Platform not supported
#endif

#include <FS.h>

	enum FSMethod { FS_DIR, FS_FILE };

	class WEBSERVERRESPONSE_CLASSNAME {

	public:
		::WEBSERVERNAME_CLASSNAME* server;

		using RequestHandlerType = WEBSERVERNAME_NAMESPACE::RequestHandler<::WEBSERVERNAME_CLASSNAME>;

		WEBSERVERRESPONSE_CLASSNAME(::WEBSERVERNAME_CLASSNAME& server);
		void allowCORS(const String allowOrigin = "*", const int maxAge = 10000, const String allowMethods = "PUT,POST,GET,DELETE,OPTIONS", const String allowHeaders = "*");
		void on(const String &uri, ::WEBSERVERNAME_CLASSNAME::THandlerFunction handler);
		void on(const String &uri, HTTPMethod method, ::WEBSERVERNAME_CLASSNAME::THandlerFunction fn);
		void on(const String &uri, HTTPMethod method, ::WEBSERVERNAME_CLASSNAME::THandlerFunction fn, WEBSERVERNAME_CLASSNAME::THandlerFunction ufn);
		void onFS(const String &uri);
		void onFS(const String &uri, FSMethod method, const String &path);
		void onNotFound(::WEBSERVERNAME_CLASSNAME::THandlerFunction fn);
		void send_P(int code, PGM_P content_type, PGM_P content);
		void send(int code, const char* contentType, const String& content);
		void send(int code);

	private:
		::WEBSERVERNAME_CLASSNAME::THandlerFunction _notFoundFunc;
		::WEBSERVERNAME_CLASSNAME::THandlerFunction _notFoundHandlerForServer;
		RequestHandlerType*  _currentHandler;
		RequestHandlerType*  _firstHandler;
		RequestHandlerType*  _lastHandler;
		String corsAllowOrigin = emptyString;
		String corsMaxAge = emptyString;
		String corsAllowMethods = emptyString;
		String corsAllowHeaders = emptyString;

		//void addFSRequestHandler(RequestHandlerType* handler);
		//RequestHandlerType* handleFSRequest(String& uri, FSMethod method);
		void handleNotFound();
		void sendHeader();
	};

typedef ::WEBSERVERNAME_CLASSNAME WebServer;
typedef WEBSERVERRESPONSE_CLASSNAME WebServerResponse;

#endif

