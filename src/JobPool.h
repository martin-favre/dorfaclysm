#pragma once
#include <algorithm>
#include <vector>

#include "Vector2DInt.h"
class PlayerRequestedJob {
 public:
  enum JobType { mine };
  PlayerRequestedJob(JobType type, const Vector2DInt& pos) : mType(type), mPos(pos){}
  JobType mType {mine};
  Vector2DInt mPos;
};

class JobPool {
 public:
  static void addJob(const PlayerRequestedJob& job) { mJobs.emplace_back(job); }
  static const std::vector<PlayerRequestedJob>& getJobs() { return mJobs; }
  static void removeJob(const PlayerRequestedJob& job) {
    auto iter = std::find_if(
        mJobs.begin(), mJobs.end(),
        [&job](const PlayerRequestedJob& x) { return &x == &job; });

    ASSERT(iter != mJobs.end(), "Job not in list!");
    mJobs.erase(iter);
  }

 private:
  static std::vector<PlayerRequestedJob> mJobs;
};