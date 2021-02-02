#include "MqttHandler.h"
#include <ESP8266WiFi.h>

WiFiClient espClient;

PubSubClient mqttClient(espClient);

bool MqttHandler::isConnected() {
  return mqttClient.connected();
}

bool MqttHandler::isEnabled() {
  return enabled;
}

bool MqttHandler::getState() {
  return mqttClient.state();
}

void MqttHandler::callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "/inbox/relay01/deviceInfo") == 0)
  {
    mqttClient.publish_P("/outbox/relay01/deviceInfo", DEVICEINFO_PAYLOAD, DEVICEINFO_PAYLOAD_SIZE, true);
    return;
  }

  if (strcmp(topic, "/inbox/relay01/set") == 0)
  {
    bool hasTinValue = false;
    for (unsigned int i = 0; i < length; i++)
    {
      if (payload[i] == 't')
      {
        hasTinValue = true;
        break;
      }
    }

    if (hasTinValue)
    {
      relay.turn_on();
      char buff[] = "{\"value\": true}";
      mqttClient.publish("/outbox/relay01/set", buff);
    }
    else
    {
      relay.turn_off();
      char buff[] = "{\"value\": false}";
      mqttClient.publish("/outbox/relay01/set", buff);
    }
    return;
  }

  if (strcmp(topic, "/inbox/relay01/toggle") == 0) {
    relay.toggle();

    if (relay.get_is_turned_on()) {
      char buff[] = "{\"value\": true}";
      mqttClient.publish("/outbox/relay01/set", buff);
    } else {
      char buff[] = "{\"value\": false}";
      mqttClient.publish("/outbox/relay01/set", buff);
    }
  }
}

void MqttHandler::reconnect()
{
  // Attempt to connect
  static unsigned long reconnectTime = 0;
  if (millis() - reconnectTime > 5000)
  {
    String clientId = "SR-relay01-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), "/outbox/relay01/lwt", 1, false, "xd"))
    {
      mqttClient.subscribe("/inbox/relay01/deviceInfo");
      mqttClient.subscribe("/inbox/relay01/set");
      mqttClient.subscribe("/inbox/relay01/toggle");
      mqttClient.publish_P("/outbox/relay01/deviceInfo", DEVICEINFO_PAYLOAD, DEVICEINFO_PAYLOAD_SIZE, false);
    }
    reconnectTime = millis();
  }
}

void MqttHandler::handle()
{
  if (!enabled)
    return;

  if (!mqttClient.connected())
  {
    reconnect();
  }

  mqttClient.loop();
}

void MqttHandler::init()
{
  mqttClient.setServer("192.168.1.6", 1883);
  mqttClient.setCallback(
    [&](char* topic, uint8_t* payload, unsigned int length){
      this->callback(topic, payload, length);
    }
  );
}
