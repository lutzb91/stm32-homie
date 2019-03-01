#pragma once

#include "Datatype.h"

class Property {
    private:
        char id[64];
        char name[64];
        bool settable;
        bool retained;
        char unit[16];
        char datatype[8];
        char format[128];
        void (*handler)(const char *) = NULL;
    public:
        Property(const char *id, const char *name, bool settable, bool retained, const char *unit, Datatype datatype, const char *format);
        char* getId() { return id; }
        char* getName() { return name; }
        bool isSettable() { return settable; }
        bool isRetained() { return retained; }
        char* getUnit() { return unit; }
        char* getDatatype() { return datatype; }
        char* getFormat() { return format; }

        void callHandler(const char *payload);

        Property& setName(const char *name);
        Property& setRetained(bool retained);
        Property& setUnit(const char *unit);
        Property& setDatatype(Datatype datatype);
        Property& setFormat(const char *format);
        Property& settableHandler(void (*handler)(const char *));
};