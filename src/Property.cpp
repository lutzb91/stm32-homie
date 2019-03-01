#include <Arduino.h>
#include "Property.h"

Property::Property(const char *id, const char *name, bool settable, bool retained, const char *unit, Datatype datatype, const char *format) {
    strlcpy(this->id, id, 64);
    setName(name);
    this->settable = settable;
    setRetained(retained);
    setUnit(unit);
    setDatatype(datatype);
    setFormat(format);
}

Property& Property::setName(const char *name) {
    strlcpy(this->name, name, 64);
    return *this;
}

Property& Property::setRetained(bool retained) {
    this->retained = retained;
    return *this;
}

Property& Property::setUnit(const char *unit) {
    strlcpy(this->unit, unit, 16);
    return *this;
}

Property& Property::setDatatype(Datatype datatype)  {
    const char *type = "string";
    switch(datatype) {
        case Datatype::STRING:
            break;
        case Datatype::INTEGER:
            type = "integer";
            break;
        case Datatype::FLOAT:
            type = "float";
            break;
        case Datatype::BOOLEAN:
            type = "boolean";
            break;
        case Datatype::ENUM:
            type = "enum";
            break;
        case Datatype::COLOR:
            type = "color";
            break;
    }
    strlcpy(this->datatype, type, 8);
    return *this;
}

Property& Property::setFormat(const char *format){
    strlcpy(this->format, format, 128);
    return *this;
}

Property& Property::settableHandler(void (*handler)(const char *)) {
    this->handler = handler;
    this->settable = true;
    return *this;
}

void Property::callHandler(const char *payload) {
    this->handler(payload);
}