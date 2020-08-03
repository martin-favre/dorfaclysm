#include "Item.h"

#include <map>

#include "BlockType.h"
#include "ItemType.h"
#include "SpriteLoader.h"
namespace {

struct ItemDefinition {
  std::string name;
  std::string description;
  Sprite* mSprite;
  uint worth;
  BlockType blockType;
};

std::vector<ItemDefinition> definitions(
    static_cast<size_t>(ItemType_NumberOfItems));

std::unique_ptr<Sprite> rockItemSprite;

void loadSprites() {
  rockItemSprite = SpriteLoader::loadSpriteByIndex(Paths::RG_TILE, {6, 2});
}

void generateDefinitions() {
  definitions[ItemType_RockBlockItem] = {"Rock Block", "A block of rock",
                                         rockItemSprite.get(), 1,
                                         BlockTypeRockBlock};
}

}  // namespace

bool Item::mInitialized{false};

void Item::initialize() {
  if (!mInitialized) {
    loadSprites();
    generateDefinitions();
    mInitialized = true;
  }
}

Item::Item(const SerializedObj& serObj)
    : mType(serObj["type"]), mCount(serObj["count"]) {
  initialize();
}

void to_json(SerializedObj& out, const Item& item) {
  out["type"] = item.getItemType();
  out["count"] = item.getCount();
}

uint32_t Item::getCount() const { return mCount; }
void Item::addItems(uint32_t count) { mCount += count; }
void Item::removeItems(uint32_t count) { addItems(-count); }
bool Item::isEmpty() const { return mCount == 0; }
const std::string& Item::getName() const { return definitions[mType].name; }
const std::string& Item::getDescription() const {
  return definitions[mType].description;
}
const Sprite& Item::getSprite() const {
  ASSERT(definitions[mType].mSprite, "No sprite");
  return *definitions[mType].mSprite;
}
uint Item::getWorth() const { return definitions[mType].worth; }
ItemType Item::getItemType() const { return mType; }

BlockType Item::getBlockType() const { return definitions[mType].blockType; }

bool Item::isPlaceable() const { return getBlockType() != BlockTypeInvalid; }

bool Item::isValid() const {
  return getItemType() != ItemType_Invalid &&
         getItemType() != ItemType_NumberOfItems;
}
