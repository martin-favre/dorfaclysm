#pragma once
#include <chrono>
class Timer{
public:
	void start();
	int64_t getElapsedMicroSeconds();
	int64_t getElapsedMilliseconds();
	int64_t getElapsedSeconds();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	std::chrono::high_resolution_clock mClock;
};