#include "AirBlock.h"

#include "Item.h"
const std::string AirBlock::mName;

const Sprite* AirBlock::getTopSprite() const { return nullptr; }
bool AirBlock::mayPassThrough() const { return true; }
const std::string& AirBlock::getName() const { return mName; }
bool AirBlock::isSeeThrough() const { return true; }
bool AirBlock::supportsJob(PlayerRequestType type) const {
  return type == requestTypePlacing;
}
bool AirBlock::mayClimbUpFrom() const { return false; }
bool AirBlock::mayWalkOnTop() const { return false; }
bool AirBlock::spawnsItem() const { return false; }
std::unique_ptr<Item> AirBlock::getItem() const { return nullptr; }
BlockType AirBlock::getType() const { return BlockType::BlockTypeAirBlock; }
