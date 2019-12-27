#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Tile.h"
#include "Vector2DInt.h"
class Vector2DInt;
class GridMap {
 public:
  GridMap(const GridMap&) = delete;
  GridMap& operator=(const GridMap&) = delete;
  bool isPosInMap(const Vector2DInt& pos) const;
  bool isTileFree(const Vector2DInt& pos) const;
  const Tile* getTile(const Vector2DInt& pos) const;
  bool setTile(const Vector2DInt& pos, const Tile& tile);
  const Vector2DInt& getSize() const;
  static GridMap& generateActiveMap(
      const Vector2DInt& size,
      std::function<void(GridMap&, const Vector2DInt&)> generator);
  static GridMap& getActiveMap();

  static constexpr Vector2DInt tileRenderSize{
      64, 64};  // How many pixels wide a tile is when rendered, before other
                // modifiers

 private:
  GridMap() = default;
  std::vector<std::vector<Tile>> mTiles;
  Vector2DInt mSize;

  static GridMap mActiveMap;
};