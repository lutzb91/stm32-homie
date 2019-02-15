#include "node.h"
#include "nodecollector.h"
#include "device.h"

std::vector<Node*> Node::nodes;

Node::Node(const char *id, const char *name, const char *type, boolean array, uint8_t size) {
    strlcpy(this->id, id, NODE_ID_LENGTH);
    strlcpy(this->name, name, NODE_NAME_LENGTH);
    strlcpy(this->type, type, NODE_TYPE_LENGTH);
    this->array = array;
    this->size = size;
    NodeCollector::instance().add(this);
}

char* Node::getId() {
    return this->id;
}

boolean Node::isArray() {
    return this->array;
}

void Node::exposeProperty(const char* id, const char* name, bool settable, bool retained, const char *unit, Datatype datatype, const char *format) {
    const char *type;
    switch(datatype) {
        case Datatype::INTEGER:
            type = "integer";
            break;
        case Datatype::FLOAT:
            type = "float";
            break;
        case Datatype::BOOLEAN:
            type = "boolean";
            break;
        case Datatype::STRING:
            type = "string";
            break;
        case Datatype::ENUM:
            type = "enum";
            break;
        case Datatype::COLOR:
            type = "color";
            break;
    }
    this->properties.push_back(new Property(id, name, settable, retained, unit, type, format));
}

void Node::setup() {
    PubSubClient& mqtt = Device::instance().getMqttClient();
    mqtt.publish(constructTopic("$name"), this->name, true);
    mqtt.publish(constructTopic("$type"), this->type, true);
    if(this->array) {
        char buffer[10];
        snprintf(buffer, 5, "0-%u", this->size-1);
        mqtt.publish(constructTopic("$array"), buffer, true);
    }
    char buffer[128];
    buffer[0] = '\0';
    for(auto property = this->properties.begin(); property != this->properties.end(); ++property) {
        Property *prop = *property;
        mqtt.publish(constructPropertyTopic(prop->getId(), "$name"), prop->getName(), true);
        mqtt.publish(constructPropertyTopic(prop->getId(), "$settable"), prop->isSettable() ? "true" : "false", true);
        mqtt.publish(constructPropertyTopic(prop->getId(), "$retained"), prop->isRetained() ? "true" : "false", true);
        mqtt.publish(constructPropertyTopic(prop->getId(), "$unit"), prop->getUnit(), true);
        mqtt.publish(constructPropertyTopic(prop->getId(), "$datatype"), prop->getDatatype(), true);
        mqtt.publish(constructPropertyTopic(prop->getId(), "$format"), prop->getFormat(), true);
        strlcat(buffer, prop->getId(), 128);
        strlcat(buffer, ",", 128);
    }
    buffer[strlen(buffer)-1] = '\0'; // remove last comma
    mqtt.publish(constructTopic("$properties"), buffer, true);
}

void Node::loop() {

}

char* Node::constructTopic(const char *topic) {
    char buffer[128];
    strcpy(buffer, this->id);
    strcat(buffer, "/");
    strcat(buffer, topic);
    return Device::constructTopic(buffer);
}

char* Node::constructPropertyTopic(const char *id, const char *topic) {
    char buffer[128];
    strcpy(buffer, id);
    strcat(buffer, "/");
    strcat(buffer, topic);
    return constructTopic(buffer);
}