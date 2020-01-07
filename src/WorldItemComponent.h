#pragma once
#include "Component.h"
#include "GameObject.h"
#include "GridMap.h"

class WorldItemComponent : public Component {
 public:
  WorldItemComponent(GameObject& gObj, const std::string& name)
      : Component(gObj), mName(name), mGridMap(GridMap::getActiveMap()) {}

  void moveFromTo(const Vector2DInt& oldPos, const Vector2DInt& newPos) {
    (void)oldPos;
    (void)newPos;
    // mGridMap.getWorldTile(oldPos).unregisterComponent(*this);
    // mGridMap.getWorldTile(newPos).registerComponent(*this);
  }

  void setup() override {
    // mGridMap.getWorldTile(owner().getPosition()).registerComponent(*this);
  }

  void teardown() override {
    // mGridMap.getWorldTile(owner().getPosition()).unregisterComponent(*this);
  }

  inline const std::string& getName() const {return mName;}

 private:
  const std::string mName;
  GridMap& mGridMap;
};