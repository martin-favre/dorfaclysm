#include "GridMap.h"
#include "AirBlock.h"
#include "Helpers.h"
#include "Logging.h"
#include "Block.h"
#include "WorldItem.h"
#include "GrassBlock.h"
GridMap GridMap::mActiveMap;

void initializeGrid(
    std::vector<std::vector<std::vector<std::unique_ptr<Block>>>>& grid,
    const Vector3DInt& size) {
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

  if (generator) {
    generator(mActiveMap, size);
  } else {
    makeGridGrass(mActiveMap.mBlocks, size);
    Logging::log(std::string(__func__) + ": generator not set");
  }
  return mActiveMap;
}

const Vector3DInt& GridMap::getSize() const { return mSize; }

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
  mBlocks[pos.z][pos.y][pos.x] = std::make_unique<AirBlock>();
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
void GridMap::setBlockAt(const Vector3DInt& pos, std::unique_ptr<Block>&& newBlock)
{
  ASSERT(newBlock.get(), "Trying set a block to null ptr");
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  mBlocks[pos.z][pos.y][pos.x] = std::move(newBlock);
}

bool GridMap::isPosFree(const Vector3DInt& pos) const {
  if (!isPosInMap(pos)) return false;
  const Block& block = getBlockAt(pos);
  return block.isPassable();
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }
