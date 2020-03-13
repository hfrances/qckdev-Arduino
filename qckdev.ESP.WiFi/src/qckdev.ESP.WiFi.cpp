/*
 Name:		qckdev.cpp
 Created:	3/3/2020 11:40:53 AM
 Author:	hfrances
 Editor:	http://www.visualmicro.com
*/

#include "qckdev.ESP.WiFi.h"

const char* WiFiModeName[] = { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA", "WIFI_MAX" };

WiFiMode StringToWiFiMode(const char* name) {

	for (size_t i = 0; i < sizeof(WiFiMode); i++)
	{
		if (String(WiFiModeName[i]) == name)
		{
			return (WiFiMode)i;
		}
	}
	return (WiFiMode)-1;
}

const char* WiFiModeToString(const WiFiMode value) {
	return WiFiModeName[(int)value];
}