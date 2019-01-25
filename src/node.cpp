#include "node.h"
#include "nodecollector.h"

std::vector<Node*> Node::nodes;

Node::Node(const char *id, boolean array) {
    strncpy(this->id, id, NODE_ID_LENGTH);
    this->array = array;
    NodeCollector::instance().add(this);
}

char* Node::getId() {
    return this->id;
}

boolean Node::isArray() {
    return this->array;
}