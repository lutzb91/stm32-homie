#pragma once

#include <Arduino.h>

enum class EventType : uint8_t {
    EVENT_MQTT_CONNECTED,
    EVENT_MQTT_CONNECT_FAILED,
    EVENT_MQTT_DISCONNECTED
};

struct Event {
    EventType type;
    int mqttState;
};