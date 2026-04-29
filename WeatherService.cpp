#include "WeatherService.h"

// set 1 to get Serial print for debugging
#define WEATHER_DEBUG 1

WeatherService::WeatherService(unsigned long updateInterval) {
    _timerDelay = updateInterval;
    _lastTime = 0;
}

void WeatherService::debugLog(String msg) {
    #if WEATHER_DEBUG
    Serial.println("[WeatherService] " + msg);
    #endif
}

bool WeatherService::update() {
    if ((millis() - _lastTime) < _timerDelay && _lastTime != 0) {
        return false;
    }

    if (WiFi.status() != WL_CONNECTED) {
        debugLog("WiFi Disconnected");
        return false;
    }

    debugLog("Fetching location...");
    String IPloc = getIPLocation();
    JSONVar location = JSON.parse(IPloc);

    if (JSON.typeof(location) == "undefined") return false;

    String loc = JSON.stringify(location["loc"]);
    int commaIndex = loc.indexOf(',');
    String lat = loc.substring(0, commaIndex);
    String lon = loc.substring(commaIndex + 1);
    lat.replace("\"", "");
    lon.replace("\"", "");

    String serverPath = "https://api.open-meteo.com/v1/forecast?latitude=" + lat +
                        "&longitude=" + lon +
                        "&current=temperature_2m,relative_humidity_2m,precipitation,wind_speed_10m";

    _jsonBuffer = httpGETRequest(serverPath.c_str());
    JSONVar weatherJSON = JSON.parse(_jsonBuffer);

    if (JSON.typeof(weatherJSON) == "undefined") {
        debugLog("Parsing Weather failed!");
        return false;
    }

    // Map JSON to struct
    _currentWeather.temperature = (double)weatherJSON["current"]["temperature_2m"];
    _currentWeather.humidity = (double)weatherJSON["current"]["relative_humidity_2m"];
    _currentWeather.precipitation = (double)weatherJSON["current"]["precipitation"];
    _currentWeather.windSpeed = (double)weatherJSON["current"]["wind_speed_10m"];
    _currentWeather.valid = true;

    _lastTime = millis();
    return true;
}

String WeatherService::getIPLocation() {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.setTimeout(5000);

    http.begin(client, "https://ipinfo.io/json");
    int code = http.GET();

    if (code <= 0) {
        http.end();
        http.begin(client, "https://ipapi.co/json");
        code = http.GET();
    }

    if (code > 0) {
        String payload = http.getString();
        http.end();
        return payload;
    }
    return "{}";
}

String WeatherService::httpGETRequest(const char* serverName) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.begin(client, serverName);
    int httpResponseCode = http.GET();

    String payload = "{}";
    if (httpResponseCode > 0) {
        payload = http.getString();
    }
    http.end();
    return payload;
}