#include "DorfController.h"

#include <memory>

#include "Astar.h"
#include "BlockBuildingRequestPool.h"
#include "GameObject.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "Helpers.h"
#include "Logging.h"
#include "MineJob.h"
#include "MiningRequestPool.h"
#include "MoveItemJob.h"
#include "MoveItemRequestPool.h"
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

std::unique_ptr<IJob> getMineJob(GridActor& gridActor) {
  MiningRequestPool& pool = MiningRequestPool::getInstance();

  if (pool.hasRequests()) {
    return std::make_unique<MineJob>(
        gridActor, pool.getClosestTo(gridActor.getCurrentPos()));
  }
  return nullptr;
}

std::unique_ptr<IJob> getBlockBuildingJob(GridActor& gridActor) {
  (void)gridActor;
  // BlockBuildingRequestPool& pool = BlockBuildingRequestPool::getInstance();
  // if(pool.hasRequests())
  // {

  // }
  return nullptr;
}

std::unique_ptr<IJob> getMoveItemJob(GridActor& gridActor) {
  MoveItemRequestPool& pool = MoveItemRequestPool::getInstance();
  if (pool.hasRequests()) {
    return std::make_unique<MoveItemJob>(
        gridActor, pool.getClosestTo(gridActor.getCurrentPos()));
  }
  (void)gridActor;
  return nullptr;
}

void DorfController::getNewJob() {
  if (!mGridActor) return;

  mJob = getMineJob(*mGridActor);
  if (!mJob) mJob = getBlockBuildingJob(*mGridActor);
  if (!mJob) mJob = getMoveItemJob(*mGridActor);
  if (!mJob) mJob = std::make_unique<WalkRandomlyJob>(*mGridActor);
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