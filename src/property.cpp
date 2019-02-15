#include <Arduino.h>
#include "property.h"

Property::Property(const char *id, const char *name, bool settable, bool retained, const char *unit, const char *datatype, const char *format) {
    strlcpy(this->id, id, 64);
    strlcpy(this->name, name, 64);
    this->settable = settable;
    this->retained = retained;
    strlcpy(this->unit, unit, 16);
    strlcpy(this->datatype, datatype, 8);
    strlcpy(this->format, format, 128);
}