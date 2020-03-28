#include "GrassBlock.h"

#include "SpriteLoader.h"
#include "Item.h"
const std::string GrassBlock::mName{"Grass"};
std::unique_ptr<Sprite> GrassBlock::mSprite;

GrassBlock::GrassBlock() {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::RG_TILE_TRANSPARENT, {6, 0},
                                              Paths::SIZE_OF_RG_TILE_TRANSPARENT);
  }
}
const Sprite* GrassBlock::getTopSprite() const { return mSprite.get(); }
bool GrassBlock::mayPassThrough() const { return false; }
bool GrassBlock::mayClimbUpFrom() const { return false; }
bool GrassBlock::mayWalkOnTop() const { return true; }
bool GrassBlock::isSeeThrough() const { return false; }
const std::string& GrassBlock::getName() const { return mName; }
bool GrassBlock::supportsJob(PlayerRequestType type) const {
  return type == PlayerRequestType::requestTypeMining;
}
bool GrassBlock::spawnsItem() const { return false; }
std::unique_ptr<Item> GrassBlock::getItem() const { return nullptr; }
