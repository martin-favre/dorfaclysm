#include "WorldTile.h"

#include <algorithm>

bool WorldTile::hasComponent(const WorldItemComponent& comp) const {
  return mComponents.end() !=
         std::find_if(
             mComponents.begin(), mComponents.end(),
             [&comp](const WorldItemComponent* x) { return x == &comp; });
}
void WorldTile::registerComponent(WorldItemComponent& component) {
  ASSERT(!hasComponent(component), "Component is already on chosen tile");
  mComponents.emplace_back(&component);
}

void WorldTile::unregisterComponent(const WorldItemComponent& comp) {
  auto iter =
      std::find_if(mComponents.begin(), mComponents.end(),
                   [&comp](const WorldItemComponent* x) { return x == &comp; });

  ASSERT(iter != mComponents.end(), "Component is not on tile!");
  mComponents.erase(iter);
}

const std::vector<WorldItemComponent*>& WorldTile::getComponents() const {
  return mComponents;
}

Tile* WorldTile::getTile() { return mTile.get(); }

const Tile* WorldTile::getTile() const { return mTile.get(); }

void WorldTile::removeTile()
{
  mTile.reset();
}

TileFloor& WorldTile::getFloor() { return mFloor; }
const TileFloor& WorldTile::getFloor() const { return mFloor; }
