#pragma once
#include "Block.h"
#include "GridMap.h"

class WorldItem {
 public:
  WorldItem(const std::string& name)
      : mName(name), mGridMap(GridMap::getActiveMap()) {}

  void moveFromTo(const Vector3DInt& oldPos, const Vector3DInt& newPos) {
    mGridMap.unregisterWorldItemAt(oldPos, this);
    mGridMap.registerWorldItemAt(newPos, this);
    mOldPos = newPos;
  }

  void update(const Vector3DInt ownerPos) {
    if (ownerPos != mOldPos) {
      moveFromTo(mOldPos, ownerPos);
    }
  }

  void setup(const Vector3DInt ownerPos) {
    mGridMap.registerWorldItemAt(ownerPos, this);
    mOldPos = ownerPos;
  }

  void teardown() { mGridMap.unregisterWorldItemAt(mOldPos, this); }

  inline const std::string& getName() const { return mName; }

 private:
  Vector3DInt mOldPos;
  const std::string mName;
  GridMap& mGridMap;
};