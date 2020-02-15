#include "StairUpDownBlock.h"

#include "SpriteLoader.h"

std::unique_ptr<Sprite> StairUpDownBlock::mSprite;
const std::string StairUpDownBlock::mName{"Up/Down Stair"};

StairUpDownBlock::StairUpDownBlock() {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {7, 6},
                                              Paths::SIZE_OF_GRASS_TILE);
  }
}

const Sprite* StairUpDownBlock::getTopSprite() const { return mSprite.get(); }
bool StairUpDownBlock::mayPassThrough() const { return true; }
bool StairUpDownBlock::mayClimbUpFrom() const { return true; }
bool StairUpDownBlock::mayWalkOnTop() const { return true; }
bool StairUpDownBlock::isSeeThrough() const { return false; }
const std::string& StairUpDownBlock::getName() const { return mName; }
bool StairUpDownBlock::supportsJob(PlayerRequestType type) const {
  (void)type;
  return false;
}

bool StairUpDownBlock::spawnsItem() const { return false; }
std::unique_ptr<Item> StairUpDownBlock::getItem() const { return nullptr; }
