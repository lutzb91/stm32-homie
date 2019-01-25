#include "simpletimer.h"

SimpleTimer::SimpleTimer() {
    this->start = millis();
    this->repeat = true;
}

bool SimpleTimer::expired() {
    if((millis() - start) >= interval) {
        if(repeat) {
            start = millis();
        }
        return true;
    }
    return false;
}

void SimpleTimer::setRepeat(bool repeat) {
    this->repeat = repeat;
}

void SimpleTimer::setIntervalAndStart(uint32_t interval) {
    this->interval = interval;
    this->start = millis();
}