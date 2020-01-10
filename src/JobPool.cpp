#include "JobPool.h"

#include <algorithm>

#include "Block.h"
#include "GridMap.h"

std::vector<PlayerRequestedJob> JobPool::mJobs;

PlayerRequestedJob::PlayerRequestedJob(JobType type, const Vector3DInt& pos)
    : mType(type), mPos(pos) {}

void JobPool::addJob(const PlayerRequestedJob& job) { mJobs.emplace_back(job); }

const std::vector<PlayerRequestedJob>& JobPool::getJobs() { return mJobs; }

void JobPool::claimJob(const PlayerRequestedJob& job) {
  auto iter =
      std::find_if(mJobs.begin(), mJobs.end(),
                   [&job](const PlayerRequestedJob& x) { return &x == &job; });
  ASSERT(iter != mJobs.end(), "Job not in list!");
  mJobs.erase(iter);
}
