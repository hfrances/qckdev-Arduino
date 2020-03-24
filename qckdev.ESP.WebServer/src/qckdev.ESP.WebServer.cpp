/*
 Name:		qckdev.cpp
 Created:	3/2/2020 5:56:52 PM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#include "qckdev.ESP.WebServer.h"


String getContentType(String filename)
{
	if (filename.endsWith(".htm")) return "text/html";
	else if (filename.endsWith(".html")) return "text/html";
	else if (filename.endsWith(".css")) return "text/css";
	else if (filename.endsWith(".js")) return "application/javascript";
	else if (filename.endsWith(".png")) return "image/png";
	else if (filename.endsWith(".gif")) return "image/gif";
	else if (filename.endsWith(".jpg")) return "image/jpeg";
	else if (filename.endsWith(".ico")) return "image/x-icon";
	else if (filename.endsWith(".xml")) return "text/xml";
	else if (filename.endsWith(".pdf")) return "application/x-pdf";
	else if (filename.endsWith(".zip")) return "application/x-zip";
	else if (filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}

void serveFile(WEBSERVERNAME_CLASSNAME& server, String path, String contentType)
{
	File file = SPIFFS.open(path, "r");
	size_t sent = server.streamFile(file, contentType);
	file.close();
}

void serveFile(WEBSERVERNAME_CLASSNAME& server, String path)
{
	serveFile(server, path, getContentType(path));
}

bool handleFileRead(WEBSERVERNAME_CLASSNAME& server, String path) {
	if (path.endsWith("/")) {
		path += "index.html";
	}
	
	Serial.println("handleFileRead: " + path);
	if (SPIFFS.exists(path)) {
		serveFile(server, path);
		return true;
	}
	else {
		String pathGz = path + ".gz";
		if (SPIFFS.exists(pathGz)) {
			server.sendHeader("Content-Encoding", "gzip");
			serveFile(server, pathGz); 
			return true;
		}
	}
	Serial.println("\tFile Not Found");
	return false;
}

WEBSERVERRESPONSE_CLASSNAME::WEBSERVERRESPONSE_CLASSNAME(::WEBSERVERNAME_CLASSNAME& server) {

	this->_server = &server;
	this->_notFoundFunc = [this] {
		if (!handleFileRead(*_server, _server->uri()))
			this->handleNotFound();
	};
	_server->onNotFound(this->_notFoundFunc);
}

void WEBSERVERRESPONSE_CLASSNAME::on(const String &uri, ::WEBSERVERNAME_CLASSNAME::THandlerFunction handler) {
	_server->on(uri, handler);
}

void WEBSERVERRESPONSE_CLASSNAME::on(const String &uri, HTTPMethod method, ::WEBSERVERNAME_CLASSNAME::THandlerFunction fn) {
	_server->on(uri, method, fn);
}

void WEBSERVERRESPONSE_CLASSNAME::on(const String &uri, HTTPMethod method, ::WEBSERVERNAME_CLASSNAME::THandlerFunction fn, ::WEBSERVERNAME_CLASSNAME::THandlerFunction ufn) {
	_server->on(uri, method, fn, ufn);
}

void WEBSERVERRESPONSE_CLASSNAME::onFS(const String &uri) {
	onFS(uri, FS_DIR, "/");
}

void WEBSERVERRESPONSE_CLASSNAME::onFS(const String &uri, FSMethod method, const String &path) {
	
}

void WEBSERVERRESPONSE_CLASSNAME::onNotFound(::WEBSERVERNAME_CLASSNAME::THandlerFunction fn) {
	_notFoundHandlerForServer = fn;
}

void WEBSERVERRESPONSE_CLASSNAME::allowCORS(const String allowOrigin, const int maxAge, const String allowMethods, const String allowHeaders) {
	this->corsAllowOrigin = allowOrigin;
	this->corsMaxAge = String(maxAge);
	this->corsAllowMethods = allowMethods;
	this->corsAllowHeaders = allowHeaders;
}

void WEBSERVERRESPONSE_CLASSNAME::send(int code, const char* contentType, const String& content) {
	this->sendHeader();
	_server->send(code, contentType, content);
}

void WEBSERVERRESPONSE_CLASSNAME::send(int code) {
	this->send(code, (const char*)0, emptyString);
}

/*void WEBSERVERRESPONSE_CLASSNAME::addRequestHandler(RequestHandlerType* handler) {
	if (!_lastHandler) {
		_firstHandler = handler;
		_lastHandler = handler;
	}
	else {
		_lastHandler->next(handler);
		_lastHandler = handler;
	}
}*/

void WEBSERVERRESPONSE_CLASSNAME::sendHeader() {
	
	if (corsAllowOrigin != emptyString) {
		_server->sendHeader("Access-Control-Allow-Origin", this->corsAllowOrigin);
		if (_server->method() == HTTP_OPTIONS) {
			_server->sendHeader("Access-Control-Max-Age", this->corsMaxAge);
			_server->sendHeader("Access-Control-Allow-Methods", this->corsAllowMethods);
			_server->sendHeader("Access-Control-Allow-Headers", this->corsAllowHeaders);
		}
	}
}

void WEBSERVERRESPONSE_CLASSNAME::handleNotFound() {
	this->sendHeader();

	if (_server->method() == HTTP_OPTIONS) {
		_server->send(204);
	}
	else {
		_notFoundHandlerForServer();
	}
}