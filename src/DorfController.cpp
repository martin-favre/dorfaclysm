#include "DorfController.h"

#include <memory>

#include "Astar.h"
#include "BlockBuildingJob.h"
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

std::unique_ptr<IJob> DorfController::unserializeJob(
    const SerializedObj& serObj) {
  if (serObj.contains("job")) {
    SerializedObj job = serObj.at("job");
    IJob::Type jobType = job.at("type");
    switch (jobType) {
      case IJob::MineJob:
        return std::make_unique<MineJob>(*mGridActor, job);
      case IJob::MoveItemJob:
        return std::make_unique<MoveItemJob>(*mGridActor, job);
      case IJob::BuildBlockJob:
        return std::make_unique<BlockBuildingJob>(*mGridActor, job);
      case IJob::WalkRandomlyJob:
        return std::make_unique<WalkRandomlyJob>(*mGridActor, job);
      default:
        ASSERT(false, "unknown jobtype");
        return nullptr;
    }
  } else {
    return nullptr;
  }
}

DorfController::DorfController(GameObject& gObj, const SerializedObj& serObj)
    : Component(gObj, serObj.at(Component::SerializeString_Parent)),
      mUnserializedJob(std::make_unique<SerializedObj>(serObj.at("job"))) {}

SerializedObj DorfController::serialize() const {
  SerializedObj out = createSerializedObj<DorfController>();
  out[Component::SerializeString_Parent] = Component::serialize();
  if (mJob) {
    out["job"] = mJob->serialize();
  } else {
    // it will be empty
  }
  return out;
}

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

  if (mUnserializedJob) {
    mJob = unserializeJob(*mUnserializedJob);
    mUnserializedJob.reset();
  } else {
    mJob = getMineJob(*mGridActor);
    if (!mJob) mJob = getBlockBuildingJob(*mGridActor);
    if (!mJob) mJob = getMoveItemJob(*mGridActor);
    if (!mJob) mJob = std::make_unique<WalkRandomlyJob>(*mGridActor);
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

std::string DorfController::getTypeString() { return "DorfController"; }