#include "DorfController.h"

#include "Astar.h"
#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
#include "Logging.h"
#include "WalkRandomlyJob.h"

DorfController::DorfController(GameObject& gObj)
    : Component(gObj), mJob(std::make_unique<WalkRandomlyJob>(owner())) {}

void DorfController::getNewJob() {
  mJob = std::make_unique<WalkRandomlyJob>(owner());
}

void DorfController::update() {
  ASSERT(mJob != nullptr, "job turned to nullptr");
  bool done = mJob->work();
  if (done) {
    getNewJob();
  }
}
