#include "RockBlockItem.h"

#include "Paths.h"
#include "RockBlock.h"
#include "SpriteLoader.h"
std::unique_ptr<Sprite> RockBlockItem::mSprite;
const std::string RockBlockItem::mName{"Rock Block"};
const std::string RockBlockItem::mDesc{"A block of Rock"};
const uint RockBlockItem::mWorth{1};
const ItemType RockBlockItem::mType{ItemType_RockBlockItem};

const std::string& RockBlockItem::getName() const { return mName; }
const std::string& RockBlockItem::getDescription() const { return mDesc; }
const Sprite& RockBlockItem::getSprite() const { return *mSprite; }
uint RockBlockItem::getWorth() const { return mWorth; }
const ItemType& RockBlockItem::getItemType() const { return mType; }

std::unique_ptr<Block> RockBlockItem::getBlock() const {
  return std::make_unique<RockBlock>();
}

bool RockBlockItem::isPlaceable() const { return true; }

const Sprite& RockBlockItem::loadSprite() const {
  if (!mSprite.get()) {
    mSprite = SpriteLoader::loadSpriteByIndex(Paths::RG_TILE, {6, 2});
  }
  return *mSprite;
}