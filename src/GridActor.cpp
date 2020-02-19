#include "GridActor.h"

#include "GameObject.h"

GridActor::GridActor(GameObject& gObj, ActorType type)
    : Component(gObj), mType(type), mGridMap(GridMap::getActiveMap()) {}

Vector3DInt GridActor::getCurrentPos() const {
  return owner().getPosition();
}

void GridActor::moveTo(const Vector3DInt& newPos) {
  mGridMap.unregisterGridActorAt(owner().getPosition(), this);
  mGridMap.registerGridActorAt(newPos, this);
  owner().setPosition(newPos);
}

void GridActor::setup() {
  mGridMap.registerGridActorAt(owner().getPosition(), this);
}

void GridActor::teardown() {
  mGridMap.unregisterGridActorAt(owner().getPosition(), this);
}

const std::string& GridActor::getName() const { return owner().name(); }

GridActor::ActorType GridActor::getType() const { return mType; }
