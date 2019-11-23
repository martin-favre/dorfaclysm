#include "Timer.h"


void Timer::start(){
	mStartTime = mClock.now();
}

int64_t Timer::getElapsedMicroSeconds(){
	return std::chrono::duration_cast<std::chrono::microseconds>(mClock.now() - mStartTime).count();
}

int64_t Timer::getElapsedMilliseconds(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(mClock.now() - mStartTime).count();
}
int64_t Timer::getElapsedSeconds(){
	return std::chrono::duration_cast<std::chrono::seconds>(mClock.now() - mStartTime).count();
}
