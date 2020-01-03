#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Vector2DInt.h"
#include "WorldTile.h"

class Tile;
class TileFloor;
class WorldItemComponent;
class Vector2DInt;
class GridMap {
 public:
  GridMap(const GridMap&) = delete;
  GridMap& operator=(const GridMap&) = delete;
  bool isPosInMap(const Vector2DInt& pos) const;
  bool isPosFree(const Vector2DInt& pos) const;
  const Vector2DInt& getSize() const;

  WorldTile& getWorldTile(const Vector2DInt& pos);
  const WorldTile& getWorldTile(const Vector2DInt& pos) const;
  const Sprite& getPosSprite(const Vector2DInt& pos) const;

  static GridMap& generateActiveMap(
      const Vector2DInt& size,
      std::function<void(GridMap&, const Vector2DInt&)> generator);
  static GridMap& getActiveMap();
  static constexpr Vector2DInt tileRenderSize{
      64, 64};  // How many pixels wide a tile is when rendered, before other
                // modifiers
  friend class MapGenerator;

 private:
  GridMap() = default;
  std::vector<std::vector<WorldTile>> mTiles;
  Vector2DInt mSize;

  static GridMap mActiveMap;
};