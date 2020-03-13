/*
 Name:		qckdev.Arduino.Temperature.h
 Created:	2/11/2020 9:59:48 AM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#ifndef _qckdev_Arduino_Temperature_h
#define _qckdev_Arduino_Temperature_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Temperature
{
public:
    static float ntcRead(int pin, const float vcc, const float maxResistance, const float rangeA, const float rangeB, const float rangeC, const float dissipationFactor);
    static float ntcRead25(int pin, const float vcc, const float nominalResistance, const float beta, const float dissipationFactor);
private:
    static float getSteinhartHart(float resistance, float rangeA, float rangeB, float rangeC);
};

#endif

