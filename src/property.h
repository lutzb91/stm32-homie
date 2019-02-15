#ifndef PROPERTY_H
#define PROPERTY_H



class Property {
    private:
        char id[64];
        char name[64];
        bool settable;
        bool retained;
        char unit[16];
        char datatype[8];
        char format[128];
    public:
        Property(const char *id, const char *name, bool settable, bool retained, const char *unit, const char *datatype, const char *format);
        char* getId() { return id; }
        char* getName() { return name; }
        bool isSettable() { return settable; }
        bool isRetained() { return retained; }
        char* getUnit() { return unit; }
        char* getDatatype() { return datatype; }
        char* getFormat() { return format; }
};
#endif