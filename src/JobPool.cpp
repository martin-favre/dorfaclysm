#include "JobPool.h"

#include <algorithm>

#include "GridMap.h"

std::vector<PlayerRequestedJob> JobPool::mJobs;

PlayerRequestedJob::PlayerRequestedJob(JobType type, const Vector2DInt& pos)
    : mType(type), mPos(pos) {}

void JobPool::addJob(const PlayerRequestedJob& job) {
  GridMap& gridMap = GridMap::getActiveMap();
  if (gridMap.isPosInMap(job.mPos)) {
    WorldTile& wTile = gridMap.getWorldTile(job.mPos);
    Tile* tile = wTile.getTile();
    if (tile) {
      if (tile->supportsJob(job.mType)) {
        if (tile->getJob() != job.mType) {
          mJobs.emplace_back(job);
        }
      }
    }
  }
}

const std::vector<PlayerRequestedJob>& JobPool::getJobs() { return mJobs; }

void JobPool::removeJob(const PlayerRequestedJob& job) {
  auto iter =
      std::find_if(mJobs.begin(), mJobs.end(),
                   [&job](const PlayerRequestedJob& x) { return &x == &job; });
  ASSERT(iter != mJobs.end(), "Job not in list!");
  mJobs.erase(iter);
}