#ifndef NODE_H
#define NODE_H

#include <vector>
#include <Arduino.h>
#include <PubSubClient.h>

#define NODE_ID_LENGTH 128

class Node {

    private:
        char id[NODE_ID_LENGTH];
        boolean array;
    public:
        Node(const char *id, boolean array);
        char *getId();
        boolean isArray();
        void setup();
        void loop();

    static std::vector<Node*> nodes;
};

#endif