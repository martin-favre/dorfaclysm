#pragma once

#include <functional>
#include <memory>
#include <vector>

// #include "Vector2DInt.h"
#include "Vector3DInt.h"
#include "Block.h"

class GridMap {
 public:
  GridMap(const GridMap&) = delete;
  GridMap& operator=(const GridMap&) = delete;
  bool isPosInMap(const Vector3DInt& pos) const;
  bool isPosFree(const Vector3DInt& pos) const;
  const Vector3DInt& getSize() const;

  void removeBlockAt(const Vector3DInt& pos);
  Block& getBlockAt(const Vector3DInt& pos);
  const Block& getBlockAt(const Vector3DInt& pos) const;

  static GridMap& generateActiveMap(
      const Vector3DInt& size,
      std::function<void(GridMap&, const Vector3DInt&)> generator);
  static GridMap& getActiveMap();
  static constexpr Vector2DInt tileRenderSize{
      64, 64};  // How many pixels wide a tile is when rendered, before other
                // modifiers
  friend class MapGenerator;

 private:
  bool isBlockValid(const Vector3DInt& pos) const;
  GridMap() = default;
  std::vector<std::vector<std::vector<std::unique_ptr<Block>>>> mBlocks;
  std::vector<std::vector<std::vector<std::vector<std::unique_ptr<Component>>>>> mComponents;
  
  Vector3DInt mSize;
  static GridMap mActiveMap;
};