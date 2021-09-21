/*
 Name:		qckdev.cpp
 Created:	2/11/2020 9:49:39 AM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#include "qckdev.Arduino.h"

qckUtilClass qckUtil;

/*
void HelloWorld() {
    Serial.println("Hello world!");
}*/

/*struct Version {

	int major = NAN;
	int minor = NAN;
	int build = NAN;
	int revision = NAN;

	Version(int major, int minor) {
		this->major = major;
		this->minor = minor;
	}

	Version(int major, int minor, int build) {
		this->major = major;
		this->minor = minor;
		this->build = build;
	}

	Version(int major, int minor, int build, int revision) {
		this->major = major;
		this->minor = minor;
		this->build = build;
		this->revision = revision;
	}

	const String toString() {
		String rdo;

		rdo = String(this->major) + "." + String(this->minor);
		if (this->build != 0 || this->revision != 0)
		{
			rdo += "." + String(this->build);
		}
		if (this->revision != 0)
		{
			rdo += "." + String(this->revision);
		}
		return rdo;
	}
};*/