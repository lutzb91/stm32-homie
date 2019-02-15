#ifndef NODE_H
#define NODE_H

#include <vector>
#include <Arduino.h>
#include <PubSubClient.h>
#include "property.h"

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
        enum class Datatype : uint8_t {
            INTEGER, FLOAT, BOOLEAN, STRING, ENUM, COLOR
        };

        Node(const char *id, const char *name, const char *type, boolean array = false, uint8_t size = 1);
        char *getId();
        boolean isArray();
        void exposeProperty(const char* id, const char* name = "", bool settable = false, bool retained = true, const char *unit = "", Datatype datatype = Datatype::STRING, const char *format = "");
        void setup();
        void loop();
        

    static std::vector<Node*> nodes;
};

#endif