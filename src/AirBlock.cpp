#include "AirBlock.h"

const std::string AirBlock::mName;

const Sprite* AirBlock::getTopSprite() const { return nullptr; }
bool AirBlock::isClickable() const { return false; }
bool AirBlock::mayPassThrough() const { return true; }
const std::string& AirBlock::getName() const { return mName; }
bool AirBlock::isSeeThrough() const { return true; }
bool AirBlock::supportsJob(PlayerRequestType type) const {
  (void)type;
  return false;
}
bool AirBlock::mayClimbUpFrom() const { return false; }
bool AirBlock::mayWalkOnTop() const { return false; }
