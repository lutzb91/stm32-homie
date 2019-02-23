#pragma once

#include <Arduino.h>

enum class EventType : uint8_t {
    EVENT_ETHERNET_CONNECTED = 1,
    EVENT_ETHERNET_CONNECT_FAILED,
    EVENT_MQTT_CONNECTED,
    EVENT_MQTT_CONNECT_FAILED
};

struct Event {
    EventType type;
    IPAddress ip;
    int mqttState;
};