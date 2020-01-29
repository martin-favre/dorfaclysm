#include "GrassBlock.h"

#include "SpriteLoader.h"

const std::string GrassBlock::mName{"Grass"};
std::unique_ptr<Sprite> GrassBlock::mSprite;

GrassBlock::GrassBlock() {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                              Paths::SIZE_OF_GRASS_TILE);
  }
}
const Sprite* GrassBlock::getTopSprite() const { return mSprite.get(); }
bool GrassBlock::isClickable() const { return true; }
bool GrassBlock::mayPassThrough() const { return false; }
bool GrassBlock::mayClimbUpFrom() const { return false; }
bool GrassBlock::mayWalkOnTop() const { return true; }
bool GrassBlock::isSeeThrough() const { return false; }
const std::string& GrassBlock::getName() const { return mName; }
bool GrassBlock::supportsJob(PlayerRequestType type) const {
  return type == PlayerRequestType::requestTypeMining;
}