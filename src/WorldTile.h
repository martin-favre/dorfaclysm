#pragma once
#include "TileFloor.h"
#include <memory>
#include "Tile.h"

class WorldItemComponent;
class WorldTile
{
  public:
  WorldTile() = default;
  WorldTile(const TileFloor& floor) : mFloor(floor){}
  bool hasComponent(const WorldItemComponent& comp) const;
  void registerComponent(WorldItemComponent& comp);
  void unregisterComponent(const WorldItemComponent& comp);
  const std::vector<WorldItemComponent*>& getComponents()const;
  Tile* getTile();
  const Tile* getTile() const;
  void removeTile();
  TileFloor& getFloor();
  const TileFloor& getFloor() const;
  
  friend class MapGenerator;
  private:
  TileFloor mFloor{TileFloor::grass};
  std::unique_ptr<Tile> mTile;
  std::vector<WorldItemComponent*> mComponents;
};