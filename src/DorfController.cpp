#include "DorfController.h"

#include "Astar.h"
#include "GameObject.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "Helpers.h"
#include "Logging.h"
#include "MineJob.h"
#include "MiningRequestPool.h"
#include "WalkRandomlyJob.h"
DorfController::DorfController(GameObject& gObj)
    : Component(gObj), mJob(std::make_unique<WalkRandomlyJob>(owner())) {}

void DorfController::setup() {
  const Vector3DInt& pos = owner().getPosition();
  mGridActor.setup(pos);
  GridMapHelpers::exploreMap(GridMap::getActiveMap(), pos);
}
void DorfController::teardown() { mGridActor.teardown(); }
void DorfController::getNewJob() {
  auto& jobs = MiningRequestPool::getJobs();
  if (jobs.size() > 0) {
    mJob = std::make_unique<MineJob>(owner(), MiningRequestPool::claimRequest(jobs.begin()));
  } else {
    mJob = std::make_unique<WalkRandomlyJob>(owner());
  }
}

void DorfController::update() {
  ASSERT(mJob != nullptr, "job turned to nullptr");
  bool done = mJob->work();
  if (done) {
    getNewJob();
  }

  mGridActor.update(owner().getPosition());
}
