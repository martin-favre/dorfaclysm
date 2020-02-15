#include "GridMap.h"

#include <queue>

#include "AirBlock.h"
#include "Block.h"
#include "DeltaPositions.h"
#include "Engine.h"
#include "GameObject.h"
#include "GrassBlock.h"
#include "GridActor.h"
#include "GridMapHelpers.h"
#include "Helpers.h"
#include "ItemContainer.h"
#include "ItemContainerObject.h"
#include "Item.h"
#include "Logging.h"

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

void makeGridGrass(
    std::vector<std::vector<std::vector<std::shared_ptr<Block>>>>& grid,
    const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        grid[z][y][x] = std::make_shared<GrassBlock>();
      }
    }
  }
}

GridMap& GridMap::generateActiveMap(
    const Vector3DInt& size,
    std::function<void(GridMap&, const Vector3DInt&)> generator) {
  Logging::log(std::stringstream() << "Generating map of size " << size);
  ASSERT(size.x > 0, "Size needs to be > 0");
  ASSERT(size.y > 0, "Size needs to be > 0");
  ASSERT(size.z > 0, "Size needs to be > 0");
  mActiveMap.mSize = size;
  initializeGrid(mActiveMap.mBlocks, size);
  initializeGrid(mActiveMap.mGridActors, size);
  if (generator) {
    generator(mActiveMap, size);
  } else {
    makeGridGrass(mActiveMap.mBlocks, size);
    Logging::log(std::string(__func__) + ": generator not set");
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

void GridMap::addItemAt(const Vector3DInt& pos, std::unique_ptr<Item>&& item)
{
  std::list<GridActor*>& actors = getGridActorsAt(pos);
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
  mBlocks[pos.z][pos.y][pos.x] = std::make_shared<AirBlock>();
  GridMapHelpers::exploreMap(*this, pos);
}

void GridMap::setBlockAt(const Vector3DInt& pos,
                         std::unique_ptr<Block>&& newBlock) {
  ASSERT(newBlock.get(), "Trying set a block to null ptr");
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  mBlocks[pos.z][pos.y][pos.x] = std::move(newBlock);
}

bool GridMap::isPosFree(const Vector3DInt& pos) const {
  return getBlockAt(pos).mayPassThrough();
}

std::list<GridActor*>& GridMap::getGridActorsAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  return mGridActors[pos.z][pos.y][pos.x];
}
const std::list<GridActor*>& GridMap::getGridActorsAt(
    const Vector3DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  return mGridActors[pos.z][pos.y][pos.x];
}
void GridMap::registerGridActorAt(const Vector3DInt& pos, GridActor* item) {
  std::list<GridActor*>& items = getGridActorsAt(pos);
  items.emplace_back(item);
}
void GridMap::unregisterGridActorAt(const Vector3DInt& pos,
                                    const GridActor* item) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  std::list<GridActor*>& items = getGridActorsAt(pos);
  const auto iter = std::find(items.begin(), items.end(), item);
  ASSERT(iter != items.end(), "Item not in list");
  items.erase(iter);
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }
