#include "DeviceInfo.h"
#include "MqttHandler.h"
#include <ESP8266WiFi.h>
#include "relay.h"
#include "settings.h"

// pin 0 - relay set
// pin 1 - led
// pin 2 - input button

#define RELAY_PIN 0
#define LED_PIN 1
#define BUTTON_PIN 2

Relay relay(RELAY_PIN);
MqttHandler mqtt(relay);

extern const char *ssid;
extern const char *password;

void beginST()
{
    WiFi.disconnect();
    delay(100);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    relay.turn_off();

    beginST();
    mqtt.init();
}

void loop(void)
{
    mqtt.handle();
}
