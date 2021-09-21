#pragma once
#include "qckdev.ESP.Updater.h"

void UpdaterExtClass::_setUpdaterError() {
	StreamString str;

	if (_serial_output) {
		::Update.printError(Serial);
	}

	::Update.printError(str);
	_updaterError = str.c_str();
}

void UpdaterExtClass::_setUpdaterError(char* message) {
	_updaterError = message;
}

UpdaterExtClass::UpdaterExtClass(const bool serial_output) {
	_serial_output = serial_output;
}

bool UpdaterExtClass::begin(OTA_COMMAND command) {
	_updaterError = emptyString;
	_command = command;

	WiFiUDP::stopAll();

	Serial.println("Command value: " + String(_command));
	if (_command == OTA_COMMAND_FS) {
		size_t fsSize = ((size_t)&_FS_end - (size_t)&_FS_start);

		close_all_fs();

		// Start with max available size.
		if (!::Update.begin(fsSize, U_FS)) {
			_setUpdaterError();
		}
	}
	else {
		uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

		// Start with max available size.
		if (!::Update.begin(maxSketchSpace, U_FLASH)) {
			_setUpdaterError();
		}
	}

	_md5.begin();
	return !::Update.hasError();
}

bool UpdaterExtClass::write(uint8_t *data, size_t len) {
	if (_serial_output) {
		Serial.printf(".");
	}

	if (::Update.write(data, len) != len) {
		_setUpdaterError();
	}

	_md5.add(data, len);
	return !::Update.hasError();
}

bool UpdaterExtClass::end(bool evenIfRemaining) {
	const char* md5chr;
	bool rdo;

	_md5.calculate();
	md5chr = _md5.toString().c_str();
	::Update.setMD5(md5chr);

	rdo = ::Update.end(evenIfRemaining);
	Serial.println();
	Serial.println("Command value: " + String(_command));
	if (rdo && (_command == OTA_COMMAND_AVR)) {
		eboot_command_clear();

		Serial.println("Begin STK500...");
	}
	return rdo;
}

OTA_COMMAND UpdaterExtClass::getCommand() {
	return _command;
}

bool UpdaterExtClass::hasError() {
	return _updaterError != emptyString;
}

String UpdaterExtClass::getErrorMessage() {
	return _updaterError;
}

UpdaterExtClass UpdateExt(true);