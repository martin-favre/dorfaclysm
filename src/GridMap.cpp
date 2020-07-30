#include "GridMap.h"

#include <cstddef>
#include <memory>
#include <queue>

#include "Block.h"
#include "BlockIdentifier.h"
#include "BlockType.h"
#include "DeltaPositions.h"
#include "Engine.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMapHelpers.h"
#include "Helpers.h"
#include "Item.h"
#include "ItemContainer.h"
#include "ItemContainerObject.h"
#include "Logging.h"
#include "Serializer.h"
#include "Vector3DInt.h"

GridMap GridMap::mActiveMap;

namespace {

constexpr size_t posToIndex(const Vector3DInt& pos, const Vector3DInt& size) {
  return pos.x + size.y * (pos.y + size.z * pos.z);
}

// constexpr Vector3DInt indexToPos(size_t index, const Vector3DInt& size) {
//   int z = index / (size.x * size.y);
//   index -= (z * size.x * size.y);
//   int y = index / size.x;
//   int x = index % size.x;
//   return Vector3DInt{x, y, z};
// }

// template <class T>
// void initializeGrid(T& grid, const Vector3DInt& size) {
//   // ech
//   for (int z = 0; z < size.z; ++z) {
//     grid.emplace_back();
//     for (int y = 0; y < size.y; ++y) {
//       grid[z].emplace_back();
//       for (int x = 0; x < size.x; ++x) {
//         grid[z][y].emplace_back();
//       }
//     }
//   }
// }

// void makeGridGrass(
//     std::vector<std::vector<std::vector<std::unique_ptr<Block>>>>& grid,
//     const Vector3DInt& size) {
//   for (int z = 0; z < size.z; ++z) {
//     for (int y = 0; y < size.y; ++y) {
//       for (int x = 0; x < size.x; ++x) {
//         grid[z][y][x] = std::make_unique<Block>(BlockTypeGrassBlock);
//       }
//     }
//   }
// }

void allocateGridActors(std::unordered_map<Vector3DInt, std::list<GridActor*>,
                                           Vector3DIntHash>& gridActors,
                        const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        gridActors[Vector3DInt{x, y, z}] = {};
      }
    }
  }
}
}  // namespace

size_t GridMap::posToIndex(const Vector3DInt& pos) const {
  return ::posToIndex(pos, mActiveMap.mSize);
}

GridMap& GridMap::generateActiveMap(
    const Vector3DInt& size,
    std::function<void(GridMap&, const Vector3DInt&)> generator) {
  LOG("Generating map of size " << size);
  ASSERT(size.x > 0, "Size needs to be > 0");
  ASSERT(size.y > 0, "Size needs to be > 0");
  ASSERT(size.z > 0, "Size needs to be > 0");
  mActiveMap.mSize = size;
  // vector<vector<vector<double>>> f(
  //     3, vector<vector<double>>(4, vector<double>(5)));

  mActiveMap.mBlocks = {static_cast<size_t>(size.x * size.y * size.x),
                        Block(BlockTypeGrassBlock)};

  // initializeGrid(mActiveMap.mBlocks, size);
  allocateGridActors(mActiveMap.mGridActors, size);
  if (generator) {
    generator(mActiveMap, size);
  } else {
    // makeGridGrass(mActiveMap.mBlocks, size);
    LOG(std::string(__func__) + ": generator not set");
  }
  return mActiveMap;
}

const Vector3DInt& GridMap::getSize() const { return mSize; }
bool GridMap::getLowestPassablePositionFrom(Vector3DInt from, Vector3DInt& out,
                                            int maxDepth) const {
  if (maxDepth == -1) maxDepth = mSize.z + 1;
  int depth = 0;
  if (!isPosInMap(from)) return false;
  if (!getBlockAt(from).mayPassThrough()) return false;
  while (from.z >= 0 && getBlockAt(from).mayPassThrough()) {
    from.z += -1;
    ++depth;
    if (depth >= maxDepth) return false;
  }
  from.z += 1;
  if (from.z == 0) return false;
  out = from;
  return true;
}

bool GridMap::isPosInMap(const Vector3DInt& pos) const {
  if (pos.x < 0) return false;
  if (pos.y < 0) return false;
  if (pos.z < 0) return false;
  if (pos.y >= mSize.y) return false;
  if (pos.x >= mSize.x) return false;
  if (pos.z >= mSize.z) return false;
  return true;
}

// Irrelevant if we don't use ptrs
bool GridMap::isBlockValid(const Vector3DInt& pos) const {
  return mBlocks[posToIndex(pos)].getIdentifier().getBlockType() !=
         BlockTypeInvalid;
}

void GridMap::addItemAt(const Vector3DInt& pos, Item&& item) {
  const std::list<GridActor*>& actors = getGridActorsAt(pos);
  ItemContainer* container{};
  for (const auto& actor : actors) {
    if (actor->getType() == GridActor::item) {
      container = actor->owner().getComponent<ItemContainer>();
    }
  }

  if (!container) {
    GameObject& gObj = Engine::addGameObject<ItemContainerObject>();
    container = gObj.getComponent<ItemContainer>();
    gObj.setPosition(pos);
  }
  ASSERT(container, "Should not be null");
  container->addItem(std::move(item));
}

void GridMap::removeBlockAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  const Block& block = getBlockAt(pos);
  if (block.spawnsItem()) {
    addItemAt(pos, Item(block.getItem()));
  }
  setBlockAt(pos, BlockTypeAirBlock);
  GridMapHelpers::exploreMap(*this, pos);
}

void GridMap::setBlockAt(const Vector3DInt& pos, BlockType newBlock) {
  std::scoped_lock lock(mLock);
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  BlockIdentifier newIdent =
      mBlocks[posToIndex(pos)].getIdentifier().generateReplacement(newBlock);
  if (mBlocks[posToIndex(pos)].getIdentifier().getVersion() > 1) {
    mModifiedBlocks.emplace(pos);
  }
  mBlocks[posToIndex(pos)] = Block(newIdent);
}

void GridMap::setBlockAt(const Vector3DInt& pos, Block&& block) {
  std::scoped_lock lock(mLock);
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  mBlocks[posToIndex(pos)] = std::move(block);
  mModifiedBlocks.emplace(pos);
}

bool GridMap::isPosFree(const Vector3DInt& pos) const {
  return getBlockAt(pos).mayPassThrough();
}

const std::list<GridActor*>& GridMap::getGridActorsAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  std::scoped_lock lock(mLock);
  if (!mGridActors.count(pos)) mGridActors[pos] = {};
  return mGridActors[pos];
}

const std::list<GridActor*>& GridMap::getGridActorsAt(
    const Vector3DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  std::scoped_lock lock(mLock);
  return mGridActors.at(pos);
}

void GridMap::registerGridActorAt(const Vector3DInt& pos, GridActor* item) {
  std::scoped_lock lock(mLock);
  mGridActors[pos].emplace_back(item);
}
void GridMap::unregisterGridActorAt(const Vector3DInt& pos,
                                    const GridActor* item) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  std::scoped_lock lock(mLock);
  std::list<GridActor*>& items = mGridActors[pos];
  const auto iter = std::find(items.begin(), items.end(), item);
  ASSERT(iter != items.end(), "Item not in list");
  items.erase(iter);
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }

void GridMap::loadActiveMap(
    const SerializedObj& serObj,
    std::function<void(GridMap&, const Vector3DInt&)> generator) {
  mActiveMap.mSize = serObj["size"];
  mActiveMap.mBlocks = {
      static_cast<size_t>(mActiveMap.mSize.x * mActiveMap.mSize.y *
                          mActiveMap.mSize.z),
      Block(BlockTypeGrassBlock)};
  generator(mActiveMap, mActiveMap.mSize);
  allocateGridActors(mActiveMap.mGridActors, mActiveMap.mSize);
  std::vector<SerializedObj> blocks = serObj["blocks"];
  for (auto serBlock : blocks) {
    Vector3DInt pos = serBlock[0];
    Block block = serBlock[1];
    mActiveMap.setBlockAt(pos, std::move(block));
  }
}

void to_json(SerializedObj& json, const GridMap& gridMap) {
  std::vector<SerializedObj> blocks;
  for (const auto& pos : gridMap.mModifiedBlocks) {
    SerializedObj block = gridMap.getBlockAt(pos);
    SerializedObj serPos = pos;
    SerializedObj pair = {serPos, block};
    blocks.emplace_back(pair);
  }
  json["blocks"] = blocks;

  json["size"] = gridMap.mSize;
}
