#include "AirBlock.h"

const std::string AirBlock::mName;

const Sprite* AirBlock::getSprite() const { return nullptr; }
bool AirBlock::isClickable() const { return false; }
bool AirBlock::isPassable() const { return true; }
const std::string& AirBlock::getName() const { return mName; }
bool AirBlock::isSeeThrough() const { return true; }
bool AirBlock::supportsJob(JobType type) const {
  (void)type;
  return false;
}

const std::vector<WorldItemComponent>& AirBlock::getComponents() const {
  return mComponents;
}

BlockMovementType AirBlock::getMovementType() const {
  return movementOnlyHorizontal;
}
