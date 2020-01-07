#include "RockBlock.h"

#include "SpriteLoader.h"

std::unique_ptr<Sprite> RockBlock::mSprite;
const std::string RockBlock::mName{"Rock"};

RockBlock::RockBlock() {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 2},
                                              Paths::SIZE_OF_GRASS_TILE);
  }
}
const Sprite* RockBlock::getSprite() const { return mSprite.get(); }
bool RockBlock::isClickable() const { return true; }
bool RockBlock::isPassable() const { return false; }
const std::string& RockBlock::getName() const { return mName; }
bool RockBlock::supportsJob(JobType type) const { return type == jobTypeMine; }
BlockMovementType RockBlock::getMovementType() const {
  return movementOnlyHorizontal;
}