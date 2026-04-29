# getWeather
Get the weather using approximate location data found from IP location.  
Made for Arduino/ESP32 etc

- Due to the accuracy of IP location, this is fine for a general forecast, ie, nearest city, is it cloudy, general temps etc
- For actually accurate weather forecasts, GPS location should be used and fed into the openMeteo API call. 
- Uses https://ipinfo.io/json or https://ipapi.co/json to get IP based location.
- Uses https://open-meteo.com/ as the weather API. Check it out to see what parameters can be fetched, there's a lot!
