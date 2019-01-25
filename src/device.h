#ifndef DEVICE_H
#define DEVICE_H

#include <PubSubClient.h>
#include <UIPEthernet.h>
#include "node.h"
#include "util/simpletimer.h"
#include "util/uptime.h"
#include "events.h"

#define BASE_TOPIC "homie"
#define BUFFER_SIZE 128

class Device {
    private:
        EthernetClient ethClient;
        PubSubClient mqttClient;
        char name[128];
        char deviceId[12];
        char macStr[18];
        uint8_t mac[6];
        char buffer[BUFFER_SIZE];
        char ip[16];
        char firmwareName[128];
        char firmwareVersion[64];

        SimpleTimer statsTimer;

        Uptime uptime;

        void generateDeviceId(uint8_t *mac);
        void createMacString(uint8_t *mac);
        char* constructTopic(const char *topic);
        std::function<void(const Event&)> eventHandler;
    public:
        Device(uint8_t *mac);
        void init();
        void loop();
        void setIp(uint32_t ip);
        void setFirmware(const char *firmwareName, const char *firmwareVersion);
        void onEvent(std::function<void(const Event&)> eventHandler);
};

#endif