// EepromHandler.h

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DeviceInfo.h"
#include "relay.h"

class MqttHandler
{
public:
  MqttHandler(Relay &relay) : relay(relay){};

  void init();

  void reconnect();

  void handle();

  bool isConnected();

  bool isEnabled();

  bool getState();

  void callback(char *topic, byte *payload, unsigned int length);

private:
  Relay relay;
  bool enabled = true;
};

#endif // MQTT_HANDLER_H
