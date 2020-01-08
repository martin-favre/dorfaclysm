#pragma once
#include "GridMap.h"
#include "Block.h"

class WorldItem {
 public:
  WorldItem(const std::string& name)
      : mName(name), mGridMap(GridMap::getActiveMap()) {}

  void moveFromTo(const Vector3DInt& oldPos, const Vector3DInt& newPos) {
    (void)oldPos;
    (void)newPos;
    mGridMap.getBlockAt(oldPos).unregisterComponent(*this);
    mGridMap.getBlockAt(newPos).registerComponent(*this);
  }
  
  void update(const Vector3DInt ownerPos)
  {
    if(ownerPos != oldPos)
    {
      moveFromTo(oldPos, ownerPos);
    }
  }

  void setup(const Vector3DInt ownerPos) {
    mGridMap.getBlockAt(ownerPos).registerComponent(*this);
    oldPos = ownerPos;
  }

  void teardown() {
    mGridMap.getBlockAt(oldPos).unregisterComponent(*this);
  }

  inline const std::string& getName() const {return mName;}

 private:
  Vector3DInt oldPos;
  const std::string mName;
  GridMap& mGridMap;
};