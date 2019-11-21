#pragma once
#include <chrono>
class Timer{
public:
	void start();
	int64_t get_elapsed_microseconds();
	int64_t get_elapsed_milliseconds();
	int64_t get_elapsed_seconds();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
	std::chrono::high_resolution_clock m_clock;
};