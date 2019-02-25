#pragma once

#include <vector>
#include <Arduino.h>
#include <PubSubClient.h>
#include "property.h"
#include "Datatype.h"

#define NODE_ID_LENGTH 128
#define NODE_NAME_LENGTH 128
#define NODE_TYPE_LENGTH 128

class Node {

    private:
        char id[NODE_ID_LENGTH];
        char name[NODE_NAME_LENGTH];
        char type[NODE_TYPE_LENGTH];
        boolean array;
        uint8_t size;

        char* constructTopic(const char *topic);
        char* constructPropertyTopic(const char *id, const char *topic);

        std::vector<Property*> properties;
    public:
        Node(const char *id, const char *name, const char *type, boolean array = false, uint8_t size = 1);
        char *getId();
        boolean isArray();
        Property& exposeProperty(const char* id, const char* name = "", bool settable = false, bool retained = true, const char *unit = "", Datatype datatype = Datatype::STRING, const char *format = "");
        std::vector<Property*> getProperties();
        void setup();
        void loop();
        

    static std::vector<Node*> nodes;
};