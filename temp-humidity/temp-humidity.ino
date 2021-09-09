/*
   Request the temperature with Telegram - polluxlabs.net
*/

//Libraries
#include "credentials.h"
#include "config.h"
#include <UniversalTelegramBot.h>
#include "Adafruit_SHT4x.h"
#include "millisDelay.h"

millisDelay checkDelay, notifDelay;

//Initialize Telegram Bot
UniversalTelegramBot bot(temp_hum_botToken, client);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
sensors_event_t humidity, temp;

float celsius = temp.temperature;
float fahrenheit = (celsius * 1.8) + 32;
float r_humidity = humidity.relative_humidity;

// relay pin constant
int RELAY_PIN = 13;

//Variable for the number of requests
int numNewRequests;

//Variable for the string of the request
String text = "";

//UserID of Sender
String chat_id = "";

//Name of Sender
String from_name = "";

//Variable for your Welcome String
String welcome = "";

//Function for new requests
void handleNewRequests(int numNewRequests) {

  for (int i = 0; i < numNewRequests; i++) { //loops through new requests

    //Checks if the sender of the message is you
    chat_id = String(bot.messages[i].chat_id);
    if (chat_id != userID) {
      bot.sendMessage(chat_id, "You aren't authorized!", "");
      continue;
    }

    //Save message
    text = bot.messages[i].text;
    Serial.println(text);

    from_name = bot.messages[i].from_name;

    if (text == "/start") {
      welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use these commmands: \n\n";
      welcome += "/measure \n";
      welcome += "/clear \n";
      bot.sendMessage(chat_id, welcome, "");
      bot.sendMessage(chat_id, "https://media.giphy.com/media/jjJAkt4gHUpK8/giphy.gif", "");
    }

    if (text == "/measure") {
      sht4.getEvent(&humidity, &temp);

      float celsius = temp.temperature;
      float fahrenheit = (celsius * 1.8) + 32;
      float r_humidity = humidity.relative_humidity;
      bot.sendMessage(chat_id, "Temperature: " + String(fahrenheit) + " ºF", "");
      bot.sendMessage(chat_id, "Humidity: " + String(r_humidity) + "%", "");
      bot.sendMessage(chat_id, "Time remaining for next notification: " + String(notifDelay.remaining() / 60000) + " minutes");
    }
    if (text == "/clear") {
      notifDelay.finish();
      bot.sendMessage(chat_id, "notifications timer cleared", "");
    }
  }
}
  AdafruitIO_Feed *temperature = io.feed("temperature");
  AdafruitIO_Feed *rh_humidity = io.feed("humidity");

void setup() {
  Serial.begin(115200);
  client.setInsecure();

  //Connect to WiFi
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("");
  Serial.println("Connected!");

/*
    // connect to io.adafruit.com
    Serial.print("Connecting to Adafruit IO");
    io.connect();

    // wait for a connection
    while(io.status() < AIO_CONNECTED) {
      Serial.print(".");
      delay(500);
    }

    // we are connected
    Serial.println();
    Serial.println(io.statusText());
*/

  if (! sht4.begin()) {
    Serial.println("Sensor not found - Check connections.");
    while (1) {}



    // You can have 3 different precisions, higher precision takes longer
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    switch (sht4.getPrecision()) {
      case SHT4X_HIGH_PRECISION:
        Serial.println("High precision");
        break;
      case SHT4X_MED_PRECISION:
        Serial.println("Med precision");
        break;
      case SHT4X_LOW_PRECISION:
        Serial.println("Low precision");
        break;
    }

    // You can have 6 different heater settings
    // higher heat and longer times uses more power
    // and reads will take longer too!
    sht4.setHeater(SHT4X_NO_HEATER);
    switch (sht4.getHeater()) {
      case SHT4X_NO_HEATER:
        Serial.println("No heater");
        break;
      case SHT4X_HIGH_HEATER_1S:
        Serial.println("High heat for 1 second");
        break;
      case SHT4X_HIGH_HEATER_100MS:
        Serial.println("High heat for 0.1 second");
        break;
      case SHT4X_MED_HEATER_1S:
        Serial.println("Medium heat for 1 second");
        break;
      case SHT4X_MED_HEATER_100MS:
        Serial.println("Medium heat for 0.1 second");
        break;
      case SHT4X_LOW_HEATER_1S:
        Serial.println("Low heat for 1 second");
        break;
      case SHT4X_LOW_HEATER_100MS:
        Serial.println("Low heat for 0.1 second");
        break;
    }
  }

  checkDelay.start(1000); // start a 1 sec delay
  notifDelay.start(10000); // start a 10 sec delay

  // setup pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  //io.run();
 bool h_trigger = false;

  if (checkDelay.justFinished()) {
    checkDelay.repeat(); // start the delay again
    //check if there is a new request
    int numNewRequests = bot.getUpdates(bot.last_message_received + 1);
    sht4.getEvent(&humidity, &temp);
    float celsius = temp.temperature;
    float fahrenheit = (celsius * 1.8) + 32;
    float r_humidity = humidity.relative_humidity;

    Serial.print("Temperature: "); Serial.print(fahrenheit); Serial.println(" degrees F");
    Serial.print("Humidity: "); Serial.print(r_humidity); Serial.println("% rH");

    //humidity triggers
    if (r_humidity > 70) {
      if (notifDelay.justFinished()) {
        bot.sendMessage(chat_id, "https://media.giphy.com/media/HmTLatwLWpTQk/giphy.gif", "");
        bot.sendMessage(chat_id, "It's too humid \n Humidity: " + String(r_humidity) + "% rH", "");
        notifDelay.start(1800000);
      }
      else {
        Serial.print("Humidity Notification Paused ");
        Serial.print(" minutes remaining");
        Serial.println(notifDelay.remaining() / 60000);
      }
    }
    if (r_humidity < 40) {
      if (notifDelay.justFinished()) {
      bot.sendMessage(chat_id, "https://media.giphy.com/media/RWHft7zDUPD0s/giphy.gif", "");
      bot.sendMessage(chat_id, "It's too dry \nHumidity: " + String(r_humidity) + "% rH", "");
        notifDelay.start(1800000);
      }
      else {
        Serial.print("Humidity Notification Paused ");
        Serial.println(notifDelay.remaining() / 60000);
        Serial.print(" minutes remaining");
      }
    }
    //temperature triggers
    if (fahrenheit > 85) {
      if (notifDelay.justFinished()) {
      bot.sendMessage(chat_id, "https://media.giphy.com/media/jjJAkt4gHUpK8/giphy.gif", "");
      bot.sendMessage(chat_id, "It's too HOT!! \nTemperature: " + String(fahrenheit) + " degrees F", "");
      notifDelay.start(1800000);
      }
      else {
        Serial.print("Temp Notification Paused ");
        Serial.println(notifDelay.remaining() / 60000);
        Serial.print(" minutes remaining");
      }
    }
    if (fahrenheit < 60) {
      if (notifDelay.justFinished()) {
      bot.sendMessage(chat_id, "https://media.giphy.com/media/e8YmiFHeDtfMs/giphy.gif", "");
      bot.sendMessage(chat_id, "It's too COLD!! \nTemperature: " + String(fahrenheit) + " degrees F", "");
        notifDelay.start(1800000);
      }    
      else {
        Serial.print("Temp Notification Paused ");
        Serial.println(notifDelay.remaining() / 60000);
        Serial.print(" minutes remaining");
      }
    }
      
    // Turn on Humidifier trigger
    if ((r_humidity < 35)&& (h_trigger = false)&& (digitalRead(RELAY_PIN) == LOW)){
    digitalWrite(RELAY_PIN, HIGH);
      h_trigger = true;
      bot.sendMessage(chat_id, "Humidifier turning on", "");
      bot.sendMessage(chat_id, "It's too dry \nHumidity: " + String(r_humidity) + "% rH", "");
    }
    if ((r_humidity > 34)&& (h_trigger = true)&& (digitalRead(RELAY_PIN) == HIGH)) {
      digitalWrite(RELAY_PIN, LOW);
      h_trigger = false;
      bot.sendMessage(chat_id, "Humidifier turning off", "");
      bot.sendMessage(chat_id, "Humidity: " + String(r_humidity) + "% rH", "");
    }

    while (numNewRequests) { //is executed if numNewRequests == 1
      Serial.println("Request received");
      handleNewRequests(numNewRequests);
      numNewRequests = bot.getUpdates(bot.last_message_received + 1);
    }
  }
}
