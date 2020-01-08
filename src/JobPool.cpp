#include "JobPool.h"

#include <algorithm>

#include "GridMap.h"
#include "Block.h"

std::vector<PlayerRequestedJob> JobPool::mJobs;

PlayerRequestedJob::PlayerRequestedJob(JobType type, const Vector3DInt& pos)
    : mType(type), mPos(pos) {}

void JobPool::addJob(const PlayerRequestedJob& job) {
  GridMap& gridMap = GridMap::getActiveMap();
  if (gridMap.isPosInMap(job.mPos)) {
    Block& block = gridMap.getBlockAt(job.mPos);
    if (block.supportsJob(job.mType)) {
      if (block.getJob() != job.mType) {
        block.assignJob(job.mType);
        mJobs.emplace_back(job);
      }
    }
  }
}

const std::vector<PlayerRequestedJob>& JobPool::getJobs() { return mJobs; }

void JobPool::claimJob(const PlayerRequestedJob& job) {
  auto iter =
      std::find_if(mJobs.begin(), mJobs.end(),
                   [&job](const PlayerRequestedJob& x) { return &x == &job; });
  ASSERT(iter != mJobs.end(), "Job not in list!");
  mJobs.erase(iter);
}
