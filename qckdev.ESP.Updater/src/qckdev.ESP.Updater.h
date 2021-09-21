#pragma once

#ifndef _qckdev_ESP_Updater_h
#define _qckdev_ESP_Updater_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <WiFiUdp.h>
#include <FS.h>
#include <eboot_command.h>
#include <flash_hal.h>
#include <MD5Builder.h>
#include <StreamString.h>


enum OTA_COMMAND {
	OTA_COMMAND_FLASH = 0,
	OTA_COMMAND_FS = 100,
	OTA_COMMAND_AVR = 500,
};

class UpdaterExtClass {

private:

	bool _serial_output;
	MD5Builder _md5;
	OTA_COMMAND _command;
	String _updaterError;

	void _setUpdaterError();
	void _setUpdaterError(char* message);

public:

	UpdaterExtClass(const bool serial_output = false);

	// handler for the file upload, get's the sketch bytes, and writes
	// them through the Update object
	bool begin(OTA_COMMAND command);
	bool write(uint8_t *data, size_t len);
	bool end(bool evenIfRemaining);

	OTA_COMMAND getCommand();
	bool hasError();
	String getErrorMessage();

};

extern UpdaterExtClass UpdateExt;

#endif