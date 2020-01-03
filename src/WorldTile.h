#pragma once
#include "TileFloor.h"
#include <memory>
#include "Tile.h"

class WorldItemComponent;
class WorldTile
{
  public:
  WorldTile() = default;
  WorldTile(std::unique_ptr<TileFloor>&& floor) : mFloor(std::move(floor)){}
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
  std::unique_ptr<TileFloor> mFloor;
  std::unique_ptr<Tile> mTile;
  std::vector<WorldItemComponent*> mComponents;
};