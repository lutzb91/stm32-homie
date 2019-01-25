#pragma once
#include "node.h"
#include <vector>

class NodeCollector {
    private:
        NodeCollector() {}
        NodeCollector( const NodeCollector& );
        NodeCollector & operator = (const NodeCollector &);
        std::vector<Node *> nodes;
    public:
        static NodeCollector& instance() {
            static NodeCollector _instance;
            return _instance;
        }
        ~NodeCollector() {}

        void add(Node *node);

        size_t size();

        Node* get(int i);
};