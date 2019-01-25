#pragma once

#include <Arduino.h>
#include <UIPEthernet.h>

/*enum class EventType : uint8_t {
    ETHERNET_CONNECTED = 1,
    ETHERNET_CONNECT_FAILED,
    MQTT_CONNECTED,
    MQTT_CONNECT_FAILED
};*/

enum class HomieEventType : uint8_t {
  STANDALONE_MODE = 1,
  CONFIGURATION_MODE,
  NORMAL_MODE,
  OTA_STARTED,
  OTA_PROGRESS,
  OTA_SUCCESSFUL,
  OTA_FAILED,
  ABOUT_TO_RESET,
  WIFI_CONNECTED,
  WIFI_DISCONNECTED,
  MQTT_READY,
  MQTT_DISCONNECTED,
  MQTT_PACKET_ACKNOWLEDGED,
  READY_TO_SLEEP
};


struct Event {
    int type;
    IPAddress ip;
    int mqttState;
};