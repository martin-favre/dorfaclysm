#include "GridMap.h"

#include "Helpers.h"
#include "Logging.h"
#include "WorldItemComponent.h"
GridMap GridMap::mActiveMap;

GridMap& GridMap::generateActiveMap(
    const Vector2DInt& size,
    std::function<void(GridMap&, const Vector2DInt&)> generator) {
  Logging::log(std::stringstream() << "Generating map of size " << size);

  ASSERT(size.x > 0, "Size needs to be > 0");
  ASSERT(size.y > 0, "Size needs to be > 0");

  size_t sizeX = static_cast<size_t>(size.x);
  size_t sizeY = static_cast<size_t>(size.y);
  for (size_t y = 0; y < sizeY; ++y) {
    std::vector<std::unique_ptr<Tile>> row;
    for (size_t x = 0; x < sizeX; ++x) {
      row.emplace_back();
    }
    mActiveMap.mTiles.emplace_back(std::move(row));
  }

    mActiveMap.mComponents = {
        sizeY, std::vector<std::vector<WorldItemComponent*>>(sizeX)};

  if (generator) {
    generator(mActiveMap, size);
  } else {
    mActiveMap.mTileFloors = {
        sizeY, std::vector<TileFloor>(sizeX, getGrassTileFloor())};
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

const Sprite& GridMap::getPosSprite(const Vector2DInt& pos) const {
  const Tile* tile = getTile(pos);
  if (tile != nullptr) {
    return tile->getSprite();
  } else {
    return getTileFloor(pos).getSprite();
  }
}
const Tile* GridMap::getTile(const Vector2DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to get tile from out of bounds");
  return mTiles[pos.y][pos.x].get();
}
const TileFloor& GridMap::getTileFloor(const Vector2DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to get tile from out of bounds");
  return mTileFloors[pos.y][pos.x];
}

bool GridMap::isPosFree(const Vector2DInt& pos) const {
  if (!isPosInMap(pos)) return false;
  bool passable = true;
  const Tile* tile = getTile(pos);
  if (tile != nullptr) {
    passable &= tile->isOpen();
  }
  passable = passable && getTileFloor(pos).canMoveOver();
  return passable;
}

bool GridMap::isComponentOnTile(const Vector2DInt& pos,
                                const WorldItemComponent& comp) {
  std::vector<WorldItemComponent*>& components = getComponentsOnTile(pos);
  return components.end() != std::find_if(components.begin(), components.end(),
                                          [&comp](const WorldItemComponent* x) {
                                            return x == &comp;
                                          });
}

std::vector<WorldItemComponent*>& GridMap::getComponentsOnTile(
    const Vector2DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to register out of bounds");
  return mComponents[pos.y][pos.x];
}

const std::vector<WorldItemComponent*>& GridMap::getComponentsOnTile(
    const Vector2DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to register out of bounds");
  return mComponents[pos.y][pos.x];
}

void GridMap::registerComponent(const Vector2DInt& pos,
                                WorldItemComponent& component) {
  ASSERT(!isComponentOnTile(pos, component),
         "Component is already on chosen tile");
  mComponents[pos.y][pos.x].emplace_back(&component);
}

void GridMap::unregisterComponent(const Vector2DInt& pos,
                                  WorldItemComponent& comp) {
  std::vector<WorldItemComponent*>& components = getComponentsOnTile(pos);
  auto iter =
      std::find_if(components.begin(), components.end(),
                   [&comp](const WorldItemComponent* x) { return x == &comp; });

  ASSERT(iter != components.end(), "Component is not on tile!");
  components.erase(iter);
}

GridMap& GridMap::getActiveMap() { return mActiveMap; }
