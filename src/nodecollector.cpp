#include "nodecollector.h"

void NodeCollector::add(Node *node) {
    this->nodes.push_back(node);
}

size_t NodeCollector::size() {
    return this->nodes.size();
}

Node* NodeCollector::get(int i) {
    return this->nodes[i];
}