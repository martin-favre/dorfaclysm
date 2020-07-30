#include "Block.h"

#include <cstddef>
#include <memory>

#include "BlockType.h"
#include "GridActor.h"
#include "ItemType.h"
#include "Paths.h"
#include "PlayerRequestType.h"
#include "SpriteLoader.h"

namespace {
struct BlockDefinition {
  std::string name;
  Sprite* sprite;
  bool mayPassThrough;
  bool mayClimbUpFrom;
  bool mayWalkOnTop;
  bool isSeeThrough;
  ItemType itemType;
};

std::unique_ptr<Sprite> grassBlockSprite;
std::unique_ptr<Sprite> rockBlockSprite;
std::unique_ptr<Sprite> stairUpDownBlockSprite;

std::vector<BlockDefinition> blockDefinitions(
    static_cast<size_t>(BlockTypeSizeOf));

void loadSprites() {
  grassBlockSprite = SpriteLoader::loadSpriteByIndex(Paths::RG_TILE, {6, 0});
  rockBlockSprite = SpriteLoader::loadSpriteByIndex(Paths::RG_TILE, {1, 0});
  stairUpDownBlockSprite =
      SpriteLoader::loadSpriteByIndex(Paths::RG_TILE, {2, 6});
}
void generateDefinitions() {
  blockDefinitions[BlockTypeAirBlock] = {
      "Air", nullptr, true, false, false, true, ItemType_Invalid};

  blockDefinitions[BlockTypeGrassBlock] = {
      "Grass", grassBlockSprite.get(), false, false, true,
      false,   ItemType_Invalid};

  blockDefinitions[BlockTypeRockBlock] = {
      "Rock", rockBlockSprite.get(), false, false, true,
      false,  ItemType_RockBlockItem};

  blockDefinitions[BlockTypeStairUpDownBlock] = {
      "Up/Down Stair", stairUpDownBlockSprite.get(), true, true, true, false,
      ItemType_Invalid};
}

}  // namespace

bool Block::mInitialized{false};

void Block::initialize() {
  if (!mInitialized) {
    loadSprites();
    generateDefinitions();
    mInitialized = true;
  }
}

Block::Block() { initialize(); }

Block::Block(BlockType type) : mIdentifier(type) { initialize(); }

Block::Block(const BlockIdentifier& identifier) : mIdentifier(identifier) {
  initialize();
}
Block::Block(const SerializedObj& block)
    : mExplored(block["explored"]), mIdentifier(block["identifier"]) {
  initialize();
}

const Sprite* Block::getTopSprite() const {
  return blockDefinitions[mIdentifier.getBlockType()].sprite;
}
bool Block::mayPassThrough() const {
  return blockDefinitions[mIdentifier.getBlockType()].mayPassThrough;
}
bool Block::mayClimbUpFrom() const {
  return blockDefinitions[mIdentifier.getBlockType()].mayClimbUpFrom;
}

bool Block::mayWalkOnTop() const {
  return blockDefinitions[mIdentifier.getBlockType()].mayWalkOnTop;
}
const std::string& Block::getName() const {
  return blockDefinitions[mIdentifier.getBlockType()].name;
}
bool Block::supportsJob(PlayerRequestType type) const {
  switch (mIdentifier.getBlockType()) {
    case BlockTypeGrassBlock:
    case BlockTypeRockBlock:
      return type == PlayerRequestType::requestTypeMining;
    case BlockTypeAirBlock:
      return type == PlayerRequestType::requestTypePlacing;
    default:
      return false;
  }
}
bool Block::isSeeThrough() const {
  return blockDefinitions[mIdentifier.getBlockType()].isSeeThrough;
}
bool Block::spawnsItem() const {
  return blockDefinitions[mIdentifier.getBlockType()].itemType !=
         ItemType_Invalid;
}

ItemType Block::getItem() const {
  return blockDefinitions[mIdentifier.getBlockType()].itemType;
}

bool Block::isExplored() const { return mExplored; }
void Block::setExplored() { mExplored = true; }
const BlockIdentifier& Block::getIdentifier() const { return mIdentifier; }

void to_json(SerializedObj& out, const Block& block) {
  out["identifier"] = block.getIdentifier();
  out["explored"] = block.isExplored();
}

void from_json(const SerializedObj& in, Block& block) {
  block.mExplored = in["explored"];
  block.mIdentifier = in["identifier"];
}