#include "DorfController.h"

#include <memory>

#include "Astar.h"
#include "GameObject.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "Helpers.h"
#include "Logging.h"
#include "MineJob.h"
#include "MiningRequestPool.h"
#include "WalkRandomlyJob.h"
DorfController::DorfController(GameObject& gObj) : Component(gObj) {}

void DorfController::setup() {
  const Vector3DInt& pos = owner().getPosition();
  mGridActor = owner().getComponent<GridActor>();
  if (mGridActor) {
    mJob = std::make_unique<WalkRandomlyJob>(*mGridActor);
  }
  GridMapHelpers::exploreMap(GridMap::getActiveMap(), pos);
}

void DorfController::getNewJob() {
  if (!mGridActor) return;
  if (MiningRequestPool::hasRequests()) {
    mJob = std::make_unique<MineJob>(
        *mGridActor, MiningRequestPool::getClosestTo(owner().getPosition()));
  } else {
    mJob = std::make_unique<WalkRandomlyJob>(*mGridActor);
  }
}

void DorfController::update() {
  if (!mJob) {
    getNewJob();
  } else {
    bool done = mJob->work();
    if (done) {
      mJob.reset();
    }
  }
}