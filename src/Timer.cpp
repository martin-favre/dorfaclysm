#include "Timer.h"


void Timer::start(){
	m_start_time = m_clock.now();
}

int64_t Timer::get_elapsed_microseconds(){
	return std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now() - m_start_time).count();
}

int64_t Timer::get_elapsed_milliseconds(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(m_clock.now() - m_start_time).count();
}
int64_t Timer::get_elapsed_seconds(){
	return std::chrono::duration_cast<std::chrono::seconds>(m_clock.now() - m_start_time).count();
}
