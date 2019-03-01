#include "Node.h"
#include "Nodecollector.h"
#include "Device.h"

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

Property* Node::getProperty(const char *id) {
    for(auto property = this->properties.begin(); property != this->properties.end(); ++property) {
        Property *prop = *property;
        if(strcmp(prop->getId(), id) == 0) {
            return prop;
        }
    }
    return NULL;
}

Property& Node::exposeProperty(const char* id, const char* name, bool settable, bool retained, const char *unit, Datatype datatype, const char *format) {
    Property* property = new Property(id, name, settable, retained, unit, datatype, format);
    this->properties.push_back(property);
    return *property;
}

void Node::sendProperty(const char* id, const char *data) {
    PubSubClient& mqtt = Device::instance().getMqttClient();
    Property *property = getProperty(id);
    if(property) {
        mqtt.publish(constructPropertyTopic(property->getId()), data, property->isRetained());
    }
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
        if(prop->isSettable()) {
            mqtt.subscribe(constructPropertyTopic(prop->getId(), "set"));
        }
    }
    buffer[strlen(buffer)-1] = '\0'; // remove last comma
    mqtt.publish(constructTopic("$properties"), buffer, true);
}

void Node::loop() {

}

std::vector<Property*> Node::getProperties() {
    return this->properties;
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

char* Node::constructPropertyTopic(const char *id) {
    char buffer[128];
    strcpy(buffer, id);
    return constructTopic(buffer);
}