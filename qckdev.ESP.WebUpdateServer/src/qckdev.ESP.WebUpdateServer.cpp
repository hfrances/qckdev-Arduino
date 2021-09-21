#pragma once
#include "qckdev.ESP.WebUpdateServer.h"

WEBUPDATESERVERNAME_CLASSNAME::WEBUPDATESERVERNAME_CLASSNAME(bool serial_debug)
{
	_serial_output = serial_debug;
	_server = NULL;
	_username = emptyString;
	_password = emptyString;
	_authenticated = false;
}

void WEBUPDATESERVERNAME_CLASSNAME::setup(WEBSERVERNAME_CLASSNAME *server)
{
	setup(server, emptyString, emptyString);
}

void WEBUPDATESERVERNAME_CLASSNAME::setup(WEBSERVERNAME_CLASSNAME *server, const String& path)
{
	setup(server, path, emptyString, emptyString);
}

void WEBUPDATESERVERNAME_CLASSNAME::setup(WEBSERVERNAME_CLASSNAME *server, const String& username, const String& password)
{
	setup(server, "/update", username, password);
}

void WEBUPDATESERVERNAME_CLASSNAME::setup(WEBSERVERNAME_CLASSNAME *server, const String& path, const String& username, const String& password) {

	_server = server;
	_server->on(path.c_str(), HTTP_POST, [&]() {
		if (!_authenticated)
			return _server->requestAuthentication();

		if (_updaterError != emptyString) {
			_server->send(200, F("text/plain"), String(F("Update error: ")) + _updaterError);
		}
		else {
			_server->client().setNoDelay(true);
			_server->send_P(200, PSTR("text/plain"), "Update Success!");
			delay(100);
			_server->client().stop();
			if (UpdateExt.getCommand() < OTA_COMMAND_AVR) {
				if (_serial_output) {
					Serial.println("Rebooting...");
				}
				ESP.restart();
			}
		}
	}, [&]() {
		// handler for the file upload, get's the sketch bytes, and writes
		// them through the Update object
		HTTPUpload& httpUpload = _server->upload();
		OTA_COMMAND command;

		if (httpUpload.name == "0" || httpUpload.name == "OTA_COMMAND_FLASH" || httpUpload.name == "FLASH" || httpUpload.name == "firmware") {
			command = OTA_COMMAND_FLASH;
		}
		else if (httpUpload.name == "100" || httpUpload.name == "OTA_COMMAND_FS" || httpUpload.name == "FS" || httpUpload.name == "filesystem") {
			command = OTA_COMMAND_FS;
		}
		else if (httpUpload.name == "500" || httpUpload.name == "OTA_COMMAND_AVR" || httpUpload.name == "AVR") {
			command = OTA_COMMAND_AVR;
		}
		if (httpUpload.status == UPLOAD_FILE_START) {
			_updaterError = String();

			Serial.println("HTTPUpdateServer by hfrances...");
			if (_serial_output) {
				Serial.setDebugOutput(true);
				Serial.printf("Test on\n");
			}

			_authenticated = (_username == emptyString || _password == emptyString);
			if (!_authenticated) {
				_server->authenticate(_username.c_str(), _password.c_str());
			}
			if (!_authenticated) {
				if (_serial_output) {
					Serial.printf("Unauthenticated Update\n");
				}
				return;
			}
			if (!UpdateExt.begin(command)) {
				_updaterError = UpdateExt.getErrorMessage();
			}
		}
		else if (_authenticated && httpUpload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {
			if (!UpdateExt.write(httpUpload.buf, httpUpload.currentSize)) {
				_updaterError = UpdateExt.getErrorMessage();
			}
		}
		else if (_authenticated && httpUpload.status == UPLOAD_FILE_END && !_updaterError.length()) {
			// true to set the size to the current progress.
			if (!UpdateExt.end(true)) {
				_updaterError = UpdateExt.getErrorMessage();
			}
			else {
				if (_serial_output) {
					Serial.printf("\nUpdate Success: %u\n", httpUpload.totalSize);
				}
			}
		}
		else if (_authenticated && httpUpload.status == UPLOAD_FILE_ABORTED) {
			if (!UpdateExt.end(false)) {
				_updaterError = UpdateExt.getErrorMessage();
			}
			if (_serial_output) {
				Serial.println("\nUpdate was aborted");
			}
		}
		delay(0);
	});

}

void WEBUPDATESERVERNAME_CLASSNAME::updateCredentials(const String& username, const String& password)
{
	_username = username;
	_password = password;
}