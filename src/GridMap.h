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
class WorldItem;
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
  bool getClosestFreePositionTo(const Vector3DInt pos, Vector3DInt& out,
                                int widthToSearch = 1) const;

  void removeBlockAt(const Vector3DInt& pos);
  Block& getBlockAt(const Vector3DInt& pos);
  const Block& getBlockAt(const Vector3DInt& pos) const;
  void setBlockAt(const Vector3DInt& pos, std::unique_ptr<Block>&& newBlock);

  std::list<WorldItem*>& getWorldItemsAt(const Vector3DInt& pos);
  const std::list<WorldItem*>& getWorldItemsAt(const Vector3DInt& pos) const;
  void registerWorldItemAt(const Vector3DInt& pos, WorldItem* item);
  void unregisterWorldItemAt(const Vector3DInt& pos, const WorldItem* item);

  static GridMap& generateActiveMap(
      const Vector3DInt& size,
      std::function<void(GridMap&, const Vector3DInt&)> generator);
  static GridMap& getActiveMap();
  static constexpr Vector2DInt tileRenderSize{
      64, 64};  // How many pixels wide a tile is when rendered, before other
                // modifiers

  void doToEachBlockInScreen(std::function<void(const Vector3DInt&, Block&)> func);

 private:
  bool isBlockValid(const Vector3DInt& pos) const;
  GridMap() = default;
  std::vector<std::vector<std::vector<std::unique_ptr<Block>>>> mBlocks;
  std::vector<std::vector<std::vector<std::list<WorldItem*>>>> mWorldItems;
  Vector3DInt mSize;
  static GridMap mActiveMap;
};