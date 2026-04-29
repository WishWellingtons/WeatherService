#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

struct WeatherData {
    double temperature;
    double humidity;
    double precipitation;
    double windSpeed;
    bool valid = false;
};

class WeatherService {
public:
    WeatherService(unsigned long updateInterval = 10000);

    // call this to use, returns true when weather updated
    bool update();

    // get data
    WeatherData getData() const { return _currentWeather; }
    String getRawJson() const { return _jsonBuffer; }

private:
    String getIPLocation();
    String httpGETRequest(const char* serverName);

    unsigned long _lastTime;
    unsigned long _timerDelay;
    String _jsonBuffer;
    WeatherData _currentWeather;

    // debugPrint
    void debugLog(String msg);
};

#endif