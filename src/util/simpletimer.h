#pragma once

#include <Arduino.h>

class SimpleTimer {
    private:
        bool repeat;
        uint32_t start;
        uint32_t interval;
    public:
        SimpleTimer();
        bool expired();
        void setIntervalAndStart(uint32_t interval);
        void setRepeat(bool repeat);
};