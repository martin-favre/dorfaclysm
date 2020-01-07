#pragma once
#include <vector>

#include "Vector2DInt.h"
enum JobType { jobTypeUnset, jobTypeMine };
class PlayerRequestedJob {
 public:
  PlayerRequestedJob(JobType type, const Vector2DInt& pos);
  JobType mType{jobTypeUnset};
  Vector2DInt mPos;
};

class JobPool {
 public:
  static void addJob(const PlayerRequestedJob& job);
  static const std::vector<PlayerRequestedJob>& getJobs();
  static void claimJob(const PlayerRequestedJob& job);
 private:
  static std::vector<PlayerRequestedJob> mJobs;
};