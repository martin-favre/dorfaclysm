#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Tile.h"
class Vector2DInt;
class GridMap {
 public:
  GridMap(const GridMap&) = delete;
  GridMap& operator=(const GridMap&) = delete;
  bool isPosInMap(const Vector2DInt& pos) const;
  bool isTileFree(const Vector2DInt& pos) const;
  const Tile* getTile(const Vector2DInt& pos) const;
  bool setTile(const Vector2DInt& pos, const Tile& tile);
  static GridMap& generateActiveMap(
      const Vector2DInt& size,
      std::function<void(GridMap&, const Vector2DInt&)> generator);
  static GridMap& getActiveMap();

 private:
  GridMap() = default;
  std::vector<std::vector<Tile>> mTiles;
  static GridMap mActiveMap;
};