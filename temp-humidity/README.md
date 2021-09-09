# Temperature-Humidity Control
![temp-hum](../images/temp_hum.JPG)

## Description
A little tool to keep track of the temperature and humidity in our piano room. The arduino sends Telegram notifications and turns of a switched outlet to turn on a humidifier if it gets too dry in the room.

## Function
1. Check the temperature every second
2. Notify via Telegram bot if humidity is +70% / -40%
3. Notify via Telegram bot if temperature is +85f / -60f
4. turn on humidifier if below 34% humidity
5. turn off humidifier when humidity reaches 40%

### Manual Telegram Commands
* `/start` print out welcom message
* `/measure` to manually check the temperature/humidity
* `/clear` to clear the notification timer and allow notifications to be printed out again

## Arduino Parts Needed
* [SHT40 Temperature & Humidity Sensor](https://www.adafruit.com/product/4885)
* [Adafruit Feather ESP8266](https://www.adafruit.com/product/2821)

## Documentation Used
* [Adafruit IO Basics: Temperature & Humidity](https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity)
* [UniversalTelegramBot](https://www.arduino.cc/reference/en/libraries/universaltelegrambot/)
* [Coding Timers and Delays in Arduino](https://www.instructables.com/Coding-Timers-and-Delays-in-Arduino/)