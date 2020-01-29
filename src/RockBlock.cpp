#include "RockBlock.h"

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
bool RockBlock::isClickable() const { return true; }
bool RockBlock::mayPassThrough() const { return false; }
bool RockBlock::mayClimbUpFrom() const { return false; }
bool RockBlock::mayWalkOnTop() const { return true; }
const std::string& RockBlock::getName() const { return mName; }
bool RockBlock::isSeeThrough() const { return false; }
bool RockBlock::supportsJob(PlayerRequestType type) const {
  return type == requestTypeMining;
}
