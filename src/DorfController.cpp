#include "DorfController.h"

#include "Astar.h"
#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
#include "Logging.h"
#include "WalkRandomlyJob.h"
#include "WorldItemComponent.h"
#include "JobPool.h"
#include "MineJob.h"
DorfController::DorfController(GameObject& gObj)
    : Component(gObj), mJob(std::make_unique<WalkRandomlyJob>(owner())) {}

void DorfController::setup() {
  mWorldItemComp = owner().getComponent<WorldItemComponent>();
  mPreviousPosition = owner().getPosition();
}

void DorfController::getNewJob() {

  auto& jobs = JobPool::getJobs();
  if(jobs.size() > 0)
  {
    if(jobs[0].mType == jobTypeMine)
    {
      mJob = std::make_unique<MineJob>(owner(), jobs[0].mPos);
    }
    JobPool::claimJob(jobs[0]);
  }
  else
  {
    mJob = std::make_unique<WalkRandomlyJob>(owner());
  }
}

void DorfController::update() {
  ASSERT(mJob != nullptr, "job turned to nullptr");
  bool done = mJob->work();
  if (done) {
    getNewJob();
  }

  if (owner().getPosition() != mPreviousPosition) {
    if (mWorldItemComp) {
      mWorldItemComp->moveFromTo(mPreviousPosition, owner().getPosition());
      mPreviousPosition = owner().getPosition();
    }
  }
}
