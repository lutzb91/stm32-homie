#include <string.h>
#include "config.h"

void Config::loadData() {
    this->mqttPort = 1883;
    strncpy(this->mqttIp, "192.168.178.102", 15);
    this->mqttIp[15] = 0;
    strncpy(this->mqttUsername, "openhabian", 32);
    this->mqttUsername[32] = 0;
    strncpy(this->mqttPassword, "openhabian", 32);
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