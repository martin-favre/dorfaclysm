#include "Block.h"

#include "WorldItem.h"

void Block::assignJob(JobType type) { mAssignedJob = type; }

void Block::unassignJob() { mAssignedJob = jobTypeUnset; }

JobType Block::getJob() const { return mAssignedJob; }
const std::vector<WorldItem*>& Block::getComponents() const {
  return mComponents;
}

void Block::registerComponent(WorldItem& comp) {
  if (supportsComponents()) {
    mComponents.emplace_back(&comp);
  }
}

void Block::unregisterComponent(WorldItem& comp) {
  if (supportsComponents()) {
    auto iter = std::find(mComponents.begin(), mComponents.end(), &comp);
    ASSERT(iter != mComponents.end(), "Component doesn't exist in map!");
    mComponents.erase(iter);
  }
}