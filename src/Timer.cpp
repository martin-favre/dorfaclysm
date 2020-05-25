#include "Timer.h"

#include <bits/stdint-intn.h>

#include <chrono>

#include "Helpers.h"

void Timer::start() { mStartTime = mClock.now(); }

int64_t Timer::getElapsedMicroSeconds() const {
  return std::chrono::duration_cast<std::chrono::microseconds>(mClock.now() -
                                                               mStartTime)
      .count();
}

int64_t Timer::getElapsedMilliseconds() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(mClock.now() -
                                                               mStartTime)
      .count();
}
int64_t Timer::getElapsedSeconds() const {
  return std::chrono::duration_cast<std::chrono::seconds>(mClock.now() -
                                                          mStartTime)
      .count();
}

void to_json(SerializedObj& serObj, const Timer& timer) {
  serObj["elapsedTimeMicro"] = timer.getElapsedMicroSeconds();
}

void from_json(const SerializedObj& serObj, Timer& timer) {
  auto now = timer.mClock.now();
  auto elapsedTime = serObj.at("elapsedTimeMicro");
  timer.mStartTime = now - std::chrono::microseconds(elapsedTime);
}