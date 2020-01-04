#pragma once
#include <vector>

#include "Vector2DInt.h"
class PlayerRequestedJob {
 public:
  enum JobType { unset, mine };
  PlayerRequestedJob(JobType type, const Vector2DInt& pos);
  JobType mType{unset};
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