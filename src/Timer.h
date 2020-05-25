#pragma once
#include "Serializer.h"
#include <chrono>
class Timer {
 public:
  void start();
  int64_t getElapsedMicroSeconds() const;
  int64_t getElapsedMilliseconds() const;
  int64_t getElapsedSeconds() const;
  friend void to_json(SerializedObj& serObj, const Timer& timer);
  friend void from_json(const SerializedObj& serObj, Timer& timer);
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
  std::chrono::high_resolution_clock mClock;
};

void to_json(SerializedObj& serObj, const Timer& timer);
void from_json(const SerializedObj& serObj, Timer& timer);