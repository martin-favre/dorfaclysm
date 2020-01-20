#pragma once

#include <functional>
#include <list>
#include <memory>
#include <vector>

#include "Vector3DInt.h"
class Block;

/**
 * Accessing map outside bounds will throw exception.
 * Use isPosInMap.
 */
class GridActor;
class GridMap {
 public:
  GridMap(const GridMap&) = delete;
  GridMap& operator=(const GridMap&) = delete;
  bool isPosInMap(const Vector3DInt& pos) const;
  bool isPosFree(const Vector3DInt& pos) const;
  const Vector3DInt& getSize() const;

  /**
   *
   */
  bool getLowestPassablePositionFrom(Vector3DInt from, Vector3DInt& out,
                                     int maxDepth = -1) const;

  void removeBlockAt(const Vector3DInt& pos);
  void setBlockAt(const Vector3DInt& pos, std::unique_ptr<Block>&& newBlock);
  inline Block& getBlockAt(const Vector3DInt& pos) {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  return *mBlocks[pos.z][pos.y][pos.x];
  }

  inline const Block& getBlockAt(const Vector3DInt& pos) const {
  ASSERT(isPosInMap(pos), "Trying to get tile out of map");
  ASSERT(isBlockValid(pos), "Block ptr is null");
  return *mBlocks[pos.z][pos.y][pos.x];
  }

  std::list<GridActor*>& getGridActorsAt(const Vector3DInt& pos);
  const std::list<GridActor*>& getGridActorsAt(const Vector3DInt& pos) const;
  void registerGridActorAt(const Vector3DInt& pos, GridActor* item);
  void unregisterGridActorAt(const Vector3DInt& pos, const GridActor* item);

  static GridMap& generateActiveMap(
      const Vector3DInt& size,
      std::function<void(GridMap&, const Vector3DInt&)> generator);
  static GridMap& getActiveMap();
  static constexpr Vector2DInt tileRenderSize{
      64, 64};  // How many pixels wide a tile is when rendered, before other
                // modifiers


 private:
  bool isBlockValid(const Vector3DInt& pos) const;
  GridMap() = default;
  std::vector<std::vector<std::vector<std::unique_ptr<Block>>>> mBlocks;
  std::vector<std::vector<std::vector<std::list<GridActor*>>>> mGridActors;
  Vector3DInt mSize;
  static GridMap mActiveMap;
};