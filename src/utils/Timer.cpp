#include "Timer.h"
#include <SDL.h>

Timer::Timer() : started_(true), paused_(false), startTime_(SDL_GetTicks()), pausedTime_(0) { }


void Timer::start() {
	started_ = true;
	paused_ = false;
	
	startTime_ = SDL_GetTicks();
	pausedTime_ = 0;
}

void Timer::stop() {
	started_ = false;
	paused_ = false;

	startTime_ = 0;
	pausedTime_ = 0;
}

void Timer::pause() {
	if (started_ && !paused_) {
		paused_ = true;
		pausedTime_ = SDL_GetTicks() - startTime_;
		startTime_ = 0;
	}
}

void Timer::unpause() {
	if (started_ && paused_) {
		paused_ = false;
		startTime_ = SDL_GetTicks() - pausedTime_;
		pausedTime_ = 0;
	}
}

uint32_t Timer::getTicks() { 
    uint32_t time = 0;

    if (started_) {
		if (paused_) time = pausedTime_;
		else time = SDL_GetTicks() - startTime_;
    }

    return time;
}

bool Timer::isStarted() { return started_; }

bool Timer::isPaused() { return paused_; }
