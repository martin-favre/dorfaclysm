#include "RockBlockItem.h"

#include "Paths.h"
#include "RockBlock.h"
#include "SpriteLoader.h"
std::unique_ptr<Sprite> RockBlockItem::mSprite;

RockBlockItem::RockBlockItem()
    : Item(getSprite(), "Rock Block", "A block of rock", 1, ItemType(typeid(*this))) {}
std::unique_ptr<Block> RockBlockItem::getBlock() const {
  return std::make_unique<RockBlock>();
}

bool RockBlockItem::isPlaceable() const { return true; }

const Sprite& RockBlockItem::loadSprite() const {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::DF_TILE, {6, 2},
                                              Paths::SIZE_OF_DF_TILE);
  }
  return *mSprite;
}