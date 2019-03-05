#include <string.h>
#include "Config.h"

void Config::loadData() {
    this->mqttPort = 1883;
    strlcpy(this->mqttIp, "192.168.178.102", 16);
    this->mqttIp[15] = 0;
    strlcpy(this->mqttUsername, "openhabian", 32);
    this->mqttUsername[32] = 0;
    strlcpy(this->mqttPassword, "openhabian", 32);
    this->mqttPassword[32] = 0;
}

char* Config::getMqttUsername() {
    return this->mqttUsername;
}
char* Config::getMqttPassword() {
    return this->mqttPassword;
}
char* Config::getMqttIp() {
    return this->mqttIp;
}
int Config::getMqttPort() {
    return this->mqttPort;
}