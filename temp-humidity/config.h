/*******************************************************************
    A telegram bot that sends you a message when ESP
    starts up

    Parts:
    D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe
    (or any ESP8266 board)

      = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/
// Add passwords
#include "credentials.h"

 
/******************************* WIFI **************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "AdafruitIO_WiFi.h"

//WiFI Credentials
//const char* ssid = "The Circle of Wi-Fi";
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASSWORD);

WiFiClientSecure client;
