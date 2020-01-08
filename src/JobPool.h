#pragma once
#include <vector>

#include "Vector3DInt.h"
enum JobType { jobTypeUnset, jobTypeMine };
class PlayerRequestedJob {
 public:
  PlayerRequestedJob(JobType type, const Vector3DInt& pos);
  JobType mType{jobTypeUnset};
  Vector3DInt mPos;
};

class JobPool {
 public:
  static void addJob(const PlayerRequestedJob& job);
  static const std::vector<PlayerRequestedJob>& getJobs();
  static void claimJob(const PlayerRequestedJob& job);
 private:
  static std::vector<PlayerRequestedJob> mJobs;
};