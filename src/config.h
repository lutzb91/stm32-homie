#pragma once

class Config {
    private:
        Config() {
            loadData();
        }
        Config( const Config& );
        Config & operator = (const Config &);

        char mqttUsername[33];
        char mqttPassword[33];
        char mqttIp[16];
        int mqttPort;

        void loadData();
    public:
        static Config& instance() {
            static Config _instance;
            return _instance;
        }
        ~Config() {}

        char* getMqttUsername();
        char* getMqttPassword();
        char* getMqttIp();
        int getMqttPort();
};