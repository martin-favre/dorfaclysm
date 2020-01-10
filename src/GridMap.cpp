#include "GridMap.h"

#include <queue>

#include "AirBlock.h"
#include "Block.h"
#include "DeltaPositions.h"
#include "GrassBlock.h"
#include "Helpers.h"
#include "Logging.h"
#include "WorldItem.h"
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
    std::vector<std::vector<std::vector<std::unique_ptr<Block>>>>& grid,
    const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        grid[z][y][x] = std::make_unique<GrassBlock>();
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
  initializeGrid(mActiveMap.mWorldItems, size);
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

bool GridMap::getClosestFreePositionTo(const Vector3DInt pos, Vector3DInt& out,
                                       int widthToSearch) const {
  std::queue<std::pair<Vector3DInt, int>> testPositions;
  testPositions.emplace(pos, 0);
  while (!testPositions.empty()) {
    const std::pair<Vector3DInt, int> current = testPositions.front();
    testPositions.pop();
    if (isPosFree(current.first)) {
      out = current.first;
      return true;
    } else {
      if (current.second <= widthToSearch) {
        for (const Vector3DInt& delta : DELTA_POSITIONS_HORIZONTAL) {
          Vector3DInt newPos = current.first + delta;
          if (isPosInMap(newPos)) {
            testPositions.emplace(newPos, current.second);
          }
        }
      }
    }
  }
  return false;
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

void GridMap::removeBlockAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  mBlocks[pos.z][pos.y][pos.x] = std::make_unique<AirBlock>(getBlockAt(pos));
}

Block& GridMap::getBlockAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  return *mBlocks[pos.z][pos.y][pos.x];
}

const Block& GridMap::getBlockAt(const Vector3DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  return *mBlocks[pos.z][pos.y][pos.x];
}
void GridMap::setBlockAt(const Vector3DInt& pos,
                         std::unique_ptr<Block>&& newBlock) {
  ASSERT(newBlock.get(), "Trying set a block to null ptr");
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  mBlocks[pos.z][pos.y][pos.x] = std::move(newBlock);
}

bool GridMap::isPosFree(const Vector3DInt& pos) const {
  if (!isPosInMap(pos)) return false;
  const Block& block = getBlockAt(pos);
  return block.mayPassThrough();
}

std::list<WorldItem*>& GridMap::getWorldItemsAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  return mWorldItems[pos.z][pos.y][pos.x];
}
const std::list<WorldItem*>& GridMap::getWorldItemsAt(
    const Vector3DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  return mWorldItems[pos.z][pos.y][pos.x];
}
void GridMap::registerWorldItemAt(const Vector3DInt& pos, WorldItem* item) {
  std::list<WorldItem*>& items = getWorldItemsAt(pos);
  items.emplace_back(item);
}
void GridMap::unregisterWorldItemAt(const Vector3DInt& pos,
                                    const WorldItem* item) {
  ASSERT(isPosInMap(pos), "Trying to access out of bounds");
  std::list<WorldItem*>& items = getWorldItemsAt(pos);
  const auto iter = std::find(items.begin(), items.end(), item);
  ASSERT(iter != items.end(), "Item not in list");
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }
