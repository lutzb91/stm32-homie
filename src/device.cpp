#include "device.h"
#include "node.h"
#include "nodecollector.h"
#include "config.h"

Device::Device(uint8_t *mac) {
    for(int i=0;i<6;i++) {
        this->mac[i] = mac[i];
    }
    generateDeviceId(mac);
    createMacString(mac);
}

void Device::generateDeviceId(uint8_t *mac) {
    //Length MAC-Length+1 -> 13
    snprintf(this->deviceId, 13 , "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void Device::createMacString(uint8_t *mac) {
    snprintf(this->macStr, 18, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void Device::setIp(uint32_t ip) {
    snprintf(this->ip, 16, "%lu.%lu.%lu.%lu", ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
}

void Device::setFirmware(const char *firmwareName, const char *firmwareVersion) {
    strncpy(this->firmwareName, firmwareName, 128);
    this->firmwareName[127] = 0;
    strncpy(this->firmwareVersion, firmwareVersion, 64);
    this->firmwareVersion[63] = 0;
}

void Device::init() {
    if(Ethernet.begin(mac) == 0) {
        //Ethernet connect failed
        for(;;) {
        delay(200);
        digitalWrite(PC13, LOW);
        delay(200);
        digitalWrite(PC13, HIGH);
        delay(200);
        digitalWrite(PC13, LOW);
        delay(200);
        digitalWrite(PC13, HIGH);
        delay(1000);
        }
    }
    IPAddress ip = Ethernet.localIP();
    this->setIp(ip);

    // Connect MQTT - Retry 3 times
    this->mqttClient.setClient(ethClient);
    this->mqttClient.setServer(Config::instance().getMqttIp(),Config::instance().getMqttPort());

    char mqttId[19];
    strncpy(mqttId, "stmqtt", 6);
    strncat(mqttId, this->deviceId, 12);
    int counter = 0;
    do {
        if(!this->mqttClient.connect(mqttId, Config::instance().getMqttUsername(), Config::instance().getMqttPassword())) {
            if(this->mqttClient.state() != MQTT_CONNECT_FAILED) {
                // other error - wait forever
                for(;;);
            }
        }
        delay(500);
        counter++;
        if(counter == 3) {
            for(;;);
        }
    } while(!this->mqttClient.connected());

    this->mqttClient.publish(constructTopic("$state"), "init", true);
    this->mqttClient.publish(constructTopic("$homie"), "3.0.0", true);
    this->mqttClient.publish(constructTopic("$name"), this->name, true);
    this->mqttClient.publish(constructTopic("$mac"), this->macStr, true);
    this->mqttClient.publish(constructTopic("$localip"), this->ip, true);
    this->mqttClient.publish(constructTopic("$fw/name"), this->firmwareName, true);
    this->mqttClient.publish(constructTopic("$fw/version"), this->firmwareVersion, true);

    //Nodes
    // memory for all nodes plus array
    char nodes[NodeCollector::instance().size()*(NODE_ID_LENGTH + 2) + NodeCollector::instance().size()-1];
    nodes[0] = 0;
    for(uint i=0;i<NodeCollector::instance().size();i++) {
        strncat(nodes, NodeCollector::instance().get(i)->getId(), NODE_ID_LENGTH);
        if(NodeCollector::instance().get(i)->isArray()) {
            strncat(nodes, "[]", 2);
        }
        if(i < NodeCollector::instance().size()-1) {
            strncat(nodes, ",", 1);
        }
    }

    this->mqttClient.publish(constructTopic("$nodes"), nodes, true);
    this->mqttClient.publish(constructTopic("$implementation"), "stm32", true);
    this->mqttClient.publish(constructTopic("$stats"), "uptime", true);
    this->mqttClient.publish(constructTopic("$stats/interval"), "60", true);
    this->mqttClient.publish(constructTopic("$state"), "ready", true);

    char uptimeStr[21];
    uptime.update();
    itoa(uptime.getSeconds(), uptimeStr, 10);
    this->mqttClient.publish(constructTopic("$stats/uptime"), uptimeStr, true);

    this->statsTimer.setIntervalAndStart(60*1000);
}

void Device::loop() {
    this->mqttClient.loop();

    if(this->statsTimer.expired()) {
        char uptimeStr[21];
        uptime.update();
        itoa(uptime.getSeconds(), uptimeStr, 10);
        this->mqttClient.publish(constructTopic("$stats/uptime"), uptimeStr, true);
    }
}

char* Device::constructTopic(const char *topic) {
    strncpy(buffer, BASE_TOPIC, sizeof(BASE_TOPIC));
    strncat(buffer, "/", 1);
    strncat(buffer, this->deviceId, 12);
    strncat(buffer, "/", 1);
    strncat(buffer, topic, strlen(topic));
    return buffer;
}

void Device::onEvent(std::function<void(const Event&)> eventHandler) {
    this->eventHandler = eventHandler;
}