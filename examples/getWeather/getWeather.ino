#include "WeatherService.h"

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Create instance: update every 30 seconds
WeatherService weather(30000); 

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
}

void loop() {
    
    if (weather.update()) { //returns true only when the timer hits and data is fetched
        WeatherData data = weather.getData();
        
        Serial.println("--- Weather Update ---");
        Serial.print("Temp: "); Serial.println(data.temperature);
        Serial.print("Humidity: "); Serial.println(data.humidity);
        Serial.println("----------------------");
    }

}