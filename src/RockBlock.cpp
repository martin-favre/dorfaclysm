#include "RockBlock.h"

#include "RockBlockItem.h"
#include "SpriteLoader.h"
std::unique_ptr<Sprite> RockBlock::mSprite;
const std::string RockBlock::mName{"Rock"};

RockBlock::RockBlock() {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 6},
                                              Paths::SIZE_OF_GRASS_TILE);
  }
}
const Sprite* RockBlock::getTopSprite() const { return mSprite.get(); }
bool RockBlock::mayPassThrough() const { return false; }
bool RockBlock::mayClimbUpFrom() const { return false; }
bool RockBlock::mayWalkOnTop() const { return true; }
const std::string& RockBlock::getName() const { return mName; }
bool RockBlock::isSeeThrough() const { return false; }
bool RockBlock::supportsJob(PlayerRequestType type) const {
  return type == requestTypeMining;
}
bool RockBlock::spawnsItem() const { return true; }

std::unique_ptr<Item> RockBlock::getItem() const {
  return std::make_unique<RockBlockItem>();
}
