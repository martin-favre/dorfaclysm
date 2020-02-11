#include "RockBlockItem.h"

#include "Paths.h"
#include "RockBlock.h"
#include "SpriteLoader.h"

RockBlockItem::RockBlockItem() : Item(getSprite(), mName, mDesc, worth) {}
std::unique_ptr<Block> RockBlockItem::getBlock() const {
  return std::make_unique<RockBlock>();
}

bool RockBlockItem::isPlaceable() const { return true; }

const Sprite& RockBlockItem::getSprite() const {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::DF_TILE, {6, 2},
                                              Paths::SIZE_OF_DF_TILE);
  }
  return *mSprite;
}