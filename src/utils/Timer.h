#pragma once

#include <stdint.h>

class Timer {
private:
    uint32_t startTime_;
    uint32_t pausedTime_;

    bool paused_;
    bool started_;

public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    uint32_t getTicks();
    bool isStarted();
    bool isPaused();

};

