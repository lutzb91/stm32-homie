#include "Device.h"
#include "Node.h"
#include "Nodecollector.h"
#include "Config.h"

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
    Device& device = Device::instance();
    strlcpy(device.firmwareName, firmwareName, 128);
    strlcpy(device.firmwareVersion, firmwareVersion, 64);
}

void Device::setName(const char *name) {
    Device& device = Device::instance();
    strncpy(device.name, name, 128);
    device.name[127] = 0;
}

void Device::callback(char* topic, byte* payload, unsigned int length) {
    Device& device = Device::instance();
    
    // check basetopic and id
    if(strncmp(topic, BASE_TOPIC, strlen(BASE_TOPIC)) != 0) {
        device.getMqttClient().publish("debug/base_topic", "wrong");
        return;
    }
    if(strncmp(topic + strlen(BASE_TOPIC) + 1, device.deviceId, 12) != 0) {
        device.getMqttClient().publish("debug/device_id", "wrong");
        return;
    }

    char *startNodeId = topic + strlen(BASE_TOPIC) + 14; // 14 is BASE_TOPIC + id (12) + 2 '/'

    // Find correct Node
    for(uint i=0;i<NodeCollector::instance().size();i++) {
        Node *node = NodeCollector::instance().get(i);
        if(strncmp(startNodeId, node->getId(), strlen(node->getId())) == 0) {

            // correct Node found
            char *startPropId = startNodeId + 1 + strlen(node->getId());

            for(auto property = node->getProperties().begin(); property != node->getProperties().end(); ++property) {
                Property *prop = *property;
                char *propertyId = prop->getId();
                if(strncmp(startPropId, propertyId, strlen(propertyId)) == 0) {
                    // correct property found
                    if(strncmp(startPropId + 1 + strlen(propertyId), "set", 3) != 0) {
                        // no '../set'
                        return;
                    }
                    if(length < 128) {
                        memcpy(device.buffer, payload, length);
                        device.buffer[length] = '\0';
                        prop->callHandler(device.buffer);
                    }
                }
            }
        }
    }
}

void Device::setup(uint8_t *mac, uint32_t ip, Client& client) {
    Device& device = Device::instance();
    for(int i=0;i<6;i++) {
        device.mac[i] = mac[i];
    }
    device.generateDeviceId(mac);
    device.createMacString(mac);

    device.ethClient = &client;

    device.setIp(ip);

    // Connect MQTT - Retry 3 times

    device.mqttClient.setClient(*device.ethClient);
    device.mqttClient.setServer(Config::instance().getMqttIp(),Config::instance().getMqttPort());
    device.mqttClient.setCallback(device.callback);

    char mqttId[19];
    strncpy(mqttId, "stmqtt", 6);
    strncat(mqttId, device.deviceId, 12);
    int counter = 0;
    do {
        if(!device.mqttClient.connect(mqttId, Config::instance().getMqttUsername(), Config::instance().getMqttPassword(), constructTopic("$state"), 1, true, "lost")) {
            if(device.mqttClient.state() != MQTT_CONNECT_FAILED) {
                Event event;
                event.type = EventType::EVENT_MQTT_CONNECT_FAILED;
                event.mqttState = device.mqttClient.state();

                device.eventHandler(event);
                // other error - wait forever
                for(;;);
            }
        }
        delay(500);
        counter++;
        if(counter == 3) {
            for(;;);
        }
    } while(!device.mqttClient.connected());

    Event event;
    event.type = EventType::EVENT_MQTT_CONNECTED;
    event.mqttState = device.mqttClient.state();

    device.eventHandler(event);

    device.mqttClient.publish(constructTopic("$state"), "init", true);
    device.mqttClient.publish(constructTopic("$homie"), "3.0.0", true);
    device.mqttClient.publish(constructTopic("$name"), device.name, true);
    device.mqttClient.publish(constructTopic("$mac"), device.macStr, true);
    device.mqttClient.publish(constructTopic("$localip"), device.ip, true);
    device.mqttClient.publish(constructTopic("$fw/name"), device.firmwareName, true);
    device.mqttClient.publish(constructTopic("$fw/version"), device.firmwareVersion, true);

    device.mqttClient.subscribe("debug/tests/#");

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

    device.mqttClient.publish(constructTopic("$nodes"), nodes, true);
    device.mqttClient.publish(constructTopic("$implementation"), "stm32", true);
    device.mqttClient.publish(constructTopic("$stats"), "uptime", true);
    device.mqttClient.publish(constructTopic("$stats/interval"), "60", true);

    char uptimeStr[21];
    device.uptime.update();
    itoa(device.uptime.getSeconds(), uptimeStr, 10);
    device.mqttClient.publish(constructTopic("$stats/uptime"), uptimeStr, true);

    device.statsTimer.setIntervalAndStart(60*1000);

    for(uint i=0;i<NodeCollector::instance().size();i++) {
        NodeCollector::instance().get(i)->setup();
    }

    device.mqttClient.publish(constructTopic("$state"), "ready", true);
}

void Device::loop() {
    Device& device = Device::instance();
    device.mqttClient.loop();

    if(device.statsTimer.expired()) {
        char uptimeStr[21];
        device.uptime.update();
        itoa(device.uptime.getSeconds(), uptimeStr, 10);
        device.mqttClient.publish(constructTopic("$stats/uptime"), uptimeStr, true);
        device.mqttClient.publish(constructTopic("$stats/interval"), "60", true);
    }

    for(uint i=0;i<NodeCollector::instance().size();i++) {
        NodeCollector::instance().get(i)->loop();
    }
}

char* Device::constructTopic(const char *topic) {
    Device& device = Device::instance();
    char* buffer = device.buffer;
    strncpy(buffer, BASE_TOPIC, sizeof(BASE_TOPIC));
    strncat(buffer, "/", 1);
    strncat(buffer, device.deviceId, 12);
    strncat(buffer, "/", 1);
    strncat(buffer, topic, strlen(topic));
    return buffer;
}

void Device::onEvent(std::function<void(const Event&)> eventHandler) {
    this->eventHandler = eventHandler;
}

PubSubClient& Device::getMqttClient() {
    return mqttClient;
}