#include "GridMap.h"

#include <cstddef>
#include <memory>
#include <queue>

#include "AirBlock.h"
#include "Block.h"
#include "BlockFactory.h"
#include "BlockIdentifier.h"
#include "BlockType.h"
#include "DeltaPositions.h"
#include "Engine.h"
#include "GameObject.h"
#include "GrassBlock.h"
#include "GridActor.h"
#include "GridMapHelpers.h"
#include "Helpers.h"
#include "Item.h"
#include "ItemContainer.h"
#include "ItemContainerObject.h"
#include "Logging.h"
#include "Vector3DInt.h"

GridMap GridMap::mActiveMap;

template <class T>
void initializeGrid(T& grid, const Vector3DInt& size) {
  // ech
  for (int z = 0; z < size.z; ++z) {
    grid.emplace_back();
    for (int y = 0; y < size.y; ++y) {
      grid[z].emplace_back();
      for (int x = 0; x < size.x; ++x) {
        grid[z][y].emplace_back();
      }
    }
  }
}


void makeGridGrass(std::vector<std::vector<std::vector<std::unique_ptr<Block>>>>& grid,
                   const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        grid[z][y][x] = std::make_unique<GrassBlock>();
      }
    }
  }
}

void allocateGridActors(std::unordered_map<Vector3DInt, std::list<GridActor*>, Vector3DIntHash>& gridActors, const Vector3DInt& size){
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        gridActors[Vector3DInt{x, y, z}] = {};
      }
    }
  }

}

GridMap& GridMap::generateActiveMap(
    const Vector3DInt& size,
    std::function<void(GridMap&, const Vector3DInt&)> generator) {
  LOG("Generating map of size " << size);
  ASSERT(size.x > 0, "Size needs to be > 0");
  ASSERT(size.y > 0, "Size needs to be > 0");
  ASSERT(size.z > 0, "Size needs to be > 0");
  mActiveMap.mSize = size;
  initializeGrid(mActiveMap.mBlocks, size);
  allocateGridActors(mActiveMap.mGridActors, size);
  if (generator) {
    generator(mActiveMap, size);
  } else {
    makeGridGrass(mActiveMap.mBlocks, size);
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

bool GridMap::isBlockValid(const Vector3DInt& pos) const {
  return mBlocks[pos.z][pos.y][pos.x].get();
}

void GridMap::addItemAt(const Vector3DInt& pos, std::unique_ptr<Item>&& item) {
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
  if (item) {
    container->addItem(std::move(item));
  }
}

void GridMap::removeBlockAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  const Block& block = getBlockAt(pos);
  if (block.spawnsItem()) {
    addItemAt(pos, block.getItem());
  }
  {
    std::scoped_lock lock(mLock);
    mBlocks[pos.z][pos.y][pos.x] = std::make_unique<AirBlock>();
  }
  GridMapHelpers::exploreMap(*this, pos);
}

void GridMap::setBlockAt(const Vector3DInt& pos, BlockType newBlock) {
  std::scoped_lock lock(mLock);
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  if (mBlocks[pos.z][pos.y][pos.x]) {
    BlockIdentifier newIdent =
        mBlocks[pos.z][pos.y][pos.x]->getIdentifier().generateReplacement(newBlock);
    mBlocks[pos.z][pos.y][pos.x] = BlockFactory::makeBlock(newIdent);
  } else {
    BlockIdentifier newIdent{newBlock};
    mBlocks[pos.z][pos.y][pos.x] = BlockFactory::makeBlock(newIdent);
  }
}

void GridMap::setBlockAt(const Vector3DInt& pos, std::unique_ptr<Block> block) {
  std::scoped_lock lock(mLock);
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  mBlocks[pos.z][pos.y][pos.x] = std::move(block);
}

bool GridMap::isPosFree(const Vector3DInt& pos) const {
  return getBlockAt(pos).mayPassThrough();
}

const std::list<GridActor*>& GridMap::getGridActorsAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  std::scoped_lock lock(mLock);
  if(!mGridActors.count(pos)) mGridActors[pos] = {};
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
