/*
 Name:		qckdev.Arduino.h
 Created:	2/11/2020 9:49:39 AM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#ifndef _qckdev_Arduino_h
#define _qckdev_Arduino_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//static void HelloWorld();

class qckUtilClass {

public:
	int freeMemory() {
#ifdef __arm__
		// should use uinstd.h to define sbrk but Due causes a conflict
		extern "C" char* sbrk(int incr);
#else  // __ARM__
		extern char *__brkval;
#endif  // __arm__

		char top;
#ifdef __arm__
		return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
		return &top - __brkval;
#else  // __arm__
		return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
	}

};

extern qckUtilClass qckUtil;

struct Version {
	int major = NAN;
	int minor = NAN;
	int build = NAN;
	int revision = NAN;

	Version() {
	}

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

	Version(String value) {
		bool isValid = true;
		String values[4];
		int dots = 0;

		for (size_t i = 0; i < value.length() && isValid; i++)
		{
			if (value[i] == '.') {
				dots++;
			}
			else if (dots < 4 && isdigit(value[i])) {
				values[dots] += value[i];
			}
			else {
				isValid = false;
			}
		}

		if (isValid)
		{
			this->major = values[0].toInt();
			if (dots > 0) {
				this->minor = values[1].toInt();
				if (dots > 1) {
					this->build = values[2].toInt();
					if (dots > 2) {
						this->revision = values[3].toInt();
					}
				}
			}
		}
	}

	bool operator==(const Version& other) const
	{
		return (this->major == other.major &&
			this->minor == other.minor &&
			this->build == other.build &&
			this->revision == other.revision);
	}

	bool operator!=(const Version& other) const
	{
		return !(*this == other);
	}

	bool operator>=(const Version& other) const {
		return
			(this->major > other.major || (this->major == other.major &&
			(this->minor > other.minor || (this->minor == other.minor &&
				(this->build > other.build || (this->build == other.build &&
				(this->revision >= other.revision)))))));
	}

	bool operator<=(const Version& other) const {
		return
			(this->major < other.major || (this->major == other.major &&
			(this->minor < other.minor || (this->minor == other.minor &&
				(this->build < other.build || (this->build == other.build &&
				(this->revision <= other.revision)))))));
	}

	bool operator>(const Version& other) const {
		return
			(this->major > other.major || (this->major == other.major &&
			(this->minor > other.minor || (this->minor == other.minor &&
				(this->build > other.build || (this->build == other.build &&
				(this->revision > other.revision)))))));
	}

	bool operator<(const Version& other) const {
		return
			(this->major < other.major || (this->major == other.major &&
			(this->minor < other.minor || (this->minor == other.minor &&
				(this->build < other.build || (this->build == other.build &&
				(this->revision < other.revision)))))));
	}

	const String toString() const {
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

	operator const String() const {
		return this->toString();
	}

	explicit operator const char* () const {
		String value = this->toString();
		char* rdo = new char[value.length() + 1];

		strcpy(rdo, value.c_str());
		return rdo;
	}


};

#endif


/* TODO: UNIT TESTS

static void TestVersion(Version a, Version b) {
	Serial.printf("a == '%s'; b == '%s'\n", (const char*)a, (const char*)b);
	Serial.printf("a == b: %s\n", String((a == b ? "true" : "false")).c_str());
	Serial.printf("a != b: %s\n", String((a != b ? "true" : "false")).c_str());
	Serial.printf("a >= b: %s\n", String((a >= b ? "true" : "false")).c_str());
	Serial.printf("a > b: %s\n", String((a > b ? "true" : "false")).c_str());
	Serial.printf("a < b: %s\n", String((a < b ? "true" : "false")).c_str());
	Serial.printf("a <= b: %s\n", String((a <= b ? "true" : "false")).c_str());
	Serial.println();
}

static void TestVersion() {
	Version a, b;

	a = Version("1.0");
	b = Version("1.0");
	TestVersion(a, b);

	a = Version("1.0");
	b = Version("1.1");
	TestVersion(a, b);

	a = Version("1.0.0.1");
	b = Version("1.0.0.1");
	TestVersion(a, b);

	a = Version("1.0.1.0");
	b = Version("1.0.0.1");
	TestVersion(a, b);

	a = Version("8.0.1.0");
	b = Version("1.0.0.1");
	TestVersion(a, b);

	Serial.println(a);
	Serial.println(b);
	Serial.println();

	String sA = a;
	String sB = b;
	Serial.println(sA);
	Serial.println(sB);
	Serial.println();
}*/
