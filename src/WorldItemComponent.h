#include "Component.h"
#include "GameObject.h"
#include "GridMap.h"

class WorldItemComponent : public Component {
 public:
  WorldItemComponent(GameObject& gObj)
      : Component(gObj), mGridMap(GridMap::getActiveMap()) {}

  void moveFromTo(const Vector2DInt& oldPos, const Vector2DInt& newPos) {
    mGridMap.unregisterComponent(oldPos, *this);
    mGridMap.registerComponent(newPos, *this);
  }

  void setup() override {
    mGridMap.registerComponent(owner().getPosition(), *this);
  }

  void teardown() override {
    mGridMap.unregisterComponent(owner().getPosition(), *this);
  }

  virtual bool isOpen() { return true; }

 private:
  GridMap& mGridMap;
};