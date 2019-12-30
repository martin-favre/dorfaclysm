#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Tile.h"
#include "TileFloor.h"
#include "Vector2DInt.h"

class WorldItemComponent;
class Vector2DInt;
class GridMap {
 public:
  GridMap(const GridMap&) = delete;
  GridMap& operator=(const GridMap&) = delete;
  bool isPosInMap(const Vector2DInt& pos) const;
  bool isPosFree(const Vector2DInt& pos) const;
  const Vector2DInt& getSize() const;

  const Sprite& getPosSprite(const Vector2DInt& pos) const;

  std::vector<WorldItemComponent*>& getComponentsOnTile(const Vector2DInt& pos);
  bool isComponentOnTile(const Vector2DInt& pos,
                         const WorldItemComponent& comp);
  void registerComponent(const Vector2DInt& pos, WorldItemComponent& component);
  void unregisterComponent(const Vector2DInt& pos,
                           WorldItemComponent& component);

  static GridMap& generateActiveMap(
      const Vector2DInt& size,
      std::function<void(GridMap&, const Vector2DInt&)> generator);
  static GridMap& getActiveMap();
  static constexpr Vector2DInt tileRenderSize{
      64, 64};  // How many pixels wide a tile is when rendered, before other
                // modifiers
  friend class MapGenerator;

 private:
  const Tile* getTile(const Vector2DInt& pos) const;
  const TileFloor& getTileFloor(const Vector2DInt& pos) const;
  GridMap() = default;
  std::vector<std::vector<TileFloor>> mTileFloors;
  std::vector<std::vector<std::unique_ptr<Tile>>> mTiles;
  std::vector<std::vector<std::vector<WorldItemComponent*>>> mComponents;
  Vector2DInt mSize;

  static GridMap mActiveMap;
};