#include "GridMap.h"

#include "Helpers.h"
#include "Logging.h"

GridMap GridMap::mActiveMap;

GridMap& GridMap::generateActiveMap(
    const Vector2DInt& size,
    std::function<void(GridMap&, const Vector2DInt&)> generator) {
  Logging::log(std::stringstream() << "Generating map of size " << size);

  ASSERT(size.x > 0, "Size needs to be > 0");
  ASSERT(size.y > 0, "Size needs to be > 0");
  size_t sizeX = size.x;
  size_t sizeY = size.y;
  mActiveMap.mTiles = {sizeY, std::vector<Tile>(sizeX)};

  if (generator) {
    generator(mActiveMap, size);
  } else {
    Logging::log(std::string(__func__) + ": generator not set");
  }
  mActiveMap.mSize = size;
  return mActiveMap;
}

const Vector2DInt& GridMap::getSize() const { return mSize; }

bool GridMap::isPosInMap(const Vector2DInt& pos) const {
  if (pos.x < 0) return false;
  if (pos.y < 0) return false;
  if ((size_t)pos.y >= mTiles.size()) return false;
  if ((size_t)pos.x >= mTiles[0].size()) return false;
  return true;
}

const Tile* GridMap::getTile(const Vector2DInt& pos) const {
  if (!isPosInMap(pos)) return nullptr;
  return &mTiles[pos.y][pos.x];
}

bool GridMap::setTile(const Vector2DInt& pos, const Tile& tile) {
  if (!isPosInMap(pos)) return false;
  mTiles[pos.y][pos.x] = tile;
  return true;
}

bool GridMap::isTileFree(const Vector2DInt& pos) const {
  if (!isPosInMap(pos)) return false;
  return getTile(pos)->isOpen();
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }
