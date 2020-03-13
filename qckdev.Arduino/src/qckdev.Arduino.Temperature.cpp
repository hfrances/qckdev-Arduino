/*
 Name:		qckdev.Arduino.Temperature.h
 Created:	2/11/2020 9:59:48 AM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#include "qckdev.Arduino.Temperature.h"

// Returns temperature in Celsius.
float Temperature::ntcRead(int pin, const float vcc, const float nominalResistance, const float rangeA, const float rangeB, const float rangeC, const float dissipationFactor)
{
	int value = analogRead(pin);
	float volts = (value / 1023.0) * vcc; // 0V to VCC in 1023 slots.
	float resistance = (nominalResistance * volts) / (vcc - volts); // calc resistance value.
	float kelvin, celsius;

	kelvin = getSteinhartHart(resistance, rangeA, rangeB, rangeC); // apply Steinhart-Hart formula and returns the value in kelvin.
	kelvin = kelvin - ((volts * volts) / (dissipationFactor * resistance)); // apply autohot fix.
	celsius = kelvin - 273.15;

	return celsius;
}

// Returns temperature in Celsius.
float Temperature::ntcRead25(int pin, const float vcc, const float nominalResistance, const float beta, const float dissipationFactor)
{
	const float NOMINALTEMPERATURE = 25.0f;

	int value = analogRead(pin);
	float volts = (value / 1023.0) * vcc; // 0V to VCC in 1023 slots.
	float resistance = (nominalResistance * volts) / (vcc - volts); // calc resistance value.
	float kelvin, celsius;

	kelvin = 1 / beta * log(resistance / nominalResistance);
	kelvin += 1.0 / (NOMINALTEMPERATURE + 273.15);
	kelvin = 1.0 / kelvin;
	kelvin = kelvin - ((volts * volts) / (dissipationFactor * resistance)); // apply autohot fix.
	celsius = kelvin - 273.15;

	return celsius;
}

// Applies Steinhart-Hart formula and returns the temperature.
float Temperature::getSteinhartHart(float resistance, float rangeA, float rangeB, float rangeC)
{
	float logR = log(resistance);
	float temp = 1 / (rangeA + rangeB * logR + rangeC * (logR * logR * logR));

	return temp;
}
