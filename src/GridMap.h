#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>
#include <vector>

#include "Block.h"
#include "BlockIdentifier.h"
#include "Vector3DInt.h"
#include "Vector3DIntHash.h"
class Block;
class GridActor;
class Item;
class Internals;

/**
 * Accessing map outside bounds will throw exception.
 * Use isPosInMap.
 */
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
  void setBlockAt(const Vector3DInt& pos, BlockType newBlock);
  void setBlockAt(const Vector3DInt& pos, Block&&);  // unittests
  void addItemAt(const Vector3DInt& pos, Item&& item);

  inline BlockIdentifier getBlockIdentifier(const Vector3DInt& pos) const {
    const Block& block = getBlockAt(pos);
    return block.getIdentifier();
  }

  inline bool blockIdentifierMatches(const BlockIdentifier& blockIdent,
                                     const Vector3DInt& pos) const {
    ASSERT(isPosInMap(pos), "Trying to get tile out of map");
    ASSERT(isBlockValid(pos), "Block ptr is null");
    return mBlocks[posToIndex(pos)].getIdentifier() == blockIdent;
  }

  inline Block& getBlockAt(const Vector3DInt& pos) {
    std::scoped_lock lock(mLock);
    ASSERT(isPosInMap(pos), "Trying to get tile out of map");
    ASSERT(isBlockValid(pos), "Block ptr is null");
    return mBlocks[posToIndex(pos)];
  }

  inline const Block& getBlockAt(const Vector3DInt& pos) const {
    std::scoped_lock lock(mLock);
    ASSERT(isPosInMap(pos), "Trying to get tile out of map");
    ASSERT(isBlockValid(pos), "Block ptr is null");
    return mBlocks[posToIndex(pos)];
  }

  const std::list<GridActor*>& getGridActorsAt(const Vector3DInt& pos);
  const std::list<GridActor*>& getGridActorsAt(const Vector3DInt& pos) const;
  void registerGridActorAt(const Vector3DInt& pos, GridActor* item);
  void unregisterGridActorAt(const Vector3DInt& pos, const GridActor* item);

  static GridMap& generateActiveMap(
      const Vector3DInt& size,
      std::function<void(GridMap&, const Vector3DInt&)> generator);
  static void loadActiveMap(
      const SerializedObj&,
      std::function<void(GridMap&, const Vector3DInt&)> generator);
  static GridMap& getActiveMap();

 private:
  friend void to_json(SerializedObj& json, const GridMap& gridMap);
  bool isBlockValid(const Vector3DInt& pos) const;
  size_t posToIndex(const Vector3DInt& pos) const;
  std::vector<Block> mBlocks;
  std::set<Vector3DInt> mModifiedBlocks;
  // std::unordered_map<Vector3DInt, Block, Vector3DIntHash> mModifiedBlocks;
  std::unordered_map<Vector3DInt, std::list<GridActor*>, Vector3DIntHash>
      mGridActors;
  GridMap() = default;

  Vector3DInt mSize;
  static GridMap mActiveMap;
  mutable std::mutex mLock;
};

void to_json(SerializedObj& json, const GridMap& gridMap);
