#include "GridMap.h"

#include "Helpers.h"
#include "Logging.h"
#include "Tile.h"
#include "TileFloor.h"
#include "WorldItemComponent.h"
GridMap GridMap::mActiveMap;

GridMap& GridMap::generateActiveMap(
    const Vector2DInt& size,
    std::function<void(GridMap&, const Vector2DInt&)> generator) {
  Logging::log(std::stringstream() << "Generating map of size " << size);

  ASSERT(size.x > 0, "Size needs to be > 0");
  ASSERT(size.y > 0, "Size needs to be > 0");


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
  if (pos.y >= mSize.y) return false;
  if (pos.x >= mSize.x) return false;
  return true;
}

WorldTile& GridMap::getWorldTile(const Vector2DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  return mTiles[pos.y][pos.x];
}

const WorldTile& GridMap::getWorldTile(const Vector2DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  return mTiles[pos.y][pos.x];
}

const Sprite& GridMap::getPosSprite(const Vector2DInt& pos) const {
  const WorldTile& worldTile = getWorldTile(pos); 
  const Tile* tile = worldTile.getTile();
  if (tile) {
    return tile->getSprite();
  } else {
    return worldTile.getFloor().getSprite();
  }
}

bool GridMap::isPosFree(const Vector2DInt& pos) const {
  if (!isPosInMap(pos)) return false;
  bool passable = true;
  const WorldTile& worldTile = getWorldTile(pos);
  
  const Tile* tile = worldTile.getTile();
  if (tile) {
    passable &= tile->isOpen();
  }
  passable = passable && worldTile.getFloor().canMoveOver();
  return passable;
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }
