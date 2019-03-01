#pragma once

#include <PubSubClient.h>
#include <Client.h>
#include "Node.h"
#include "Property.h"
#include "util/Simpletimer.h"
#include "util/Uptime.h"
#include "Events.h"

#define BASE_TOPIC "homie"
#define BUFFER_SIZE 128

class Device {
    friend class ::Node;
    friend class ::Property;

    private:
        Client *ethClient;
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
        static char* constructTopic(const char *topic);
        std::function<void(const Event&)> eventHandler = NULL;

        void setIp(uint32_t ip);

        PubSubClient& getMqttClient();

        static void callback(char* topic, byte* payload, unsigned int length);

        Device() {}
        Device( const Device& );
        Device & operator = (const Device &);

    public:
        static void setup(uint8_t *mac, uint32_t ip, Client& client);
        static void loop();
        static void setFirmware(const char *firmwareName, const char *firmwareVersion);
        static void setName(const char *name);

        void onEvent(std::function<void(const Event&)> eventHandler);

        static Device& instance() {
            static Device _instance;
            return _instance;
        }
        ~Device() {}
};