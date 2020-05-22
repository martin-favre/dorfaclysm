#include "GridActor.h"

#include "GameObject.h"

GridActor::GridActor(GameObject& gObj, ActorType type)
    : Component(gObj), mType(type), mGridMap(GridMap::getActiveMap()) {}

GridActor::GridActor(GameObject& owner, const SerializedObj& serObj)
    : Component(owner, serObj.at(SerializeString_Parent)),
      mType(serObj.at("type")),
      mGridMap(GridMap::getActiveMap()) {}

SerializedObj GridActor::serialize() const {
  SerializedObj out = createSerializedObj<GridActor>();
  out[SerializeString_Parent] = Component::serialize();
  out["type"] = mType;
  return out;
}

Vector3DInt GridActor::getCurrentPos() const { return owner().getPosition(); }

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
