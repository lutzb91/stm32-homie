#pragma once

#include <Arduino.h>

class Uptime {
    private:
        uint64_t _milliseconds;
        uint32_t _lastTick;
    public:
        Uptime();
        void update();
        uint64_t getSeconds() const;
};