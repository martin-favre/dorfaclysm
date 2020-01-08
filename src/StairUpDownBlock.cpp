#include "StairUpDownBlock.h"

#include "SpriteLoader.h"

std::unique_ptr<Sprite> StairUpDownBlock::mSprite;
const std::string StairUpDownBlock::mName{"Up/Down Stair"};

StairUpDownBlock::StairUpDownBlock() {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                              Paths::SIZE_OF_GRASS_TILE);
  }
}

const Sprite* StairUpDownBlock::getSprite() const { return mSprite.get(); }
bool StairUpDownBlock::isClickable() const { return true; }
bool StairUpDownBlock::isPassable() const { return true; }
const std::string& StairUpDownBlock::getName() const { return mName; }
bool StairUpDownBlock::supportsJob(JobType type) const {
  (void)type;
  return false;
}
BlockMovementType StairUpDownBlock::getMovementType() const {
  return movementUpAndDownBlock;
}
