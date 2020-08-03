#include "GridActor.h"

#include "GameObject.h"
#include "Vector2DInt.h"
#include "Vector3DInt.h"

GridActor::GridActor(GameObject& gObj, ActorType type, const Vector2DInt& size)
    : Component(gObj),
      mType(type),
      mSize(size),
      mGridMap(GridMap::getActiveMap()) {}

GridActor::GridActor(GameObject& owner, const SerializedObj& serObj)
    : Component(owner, serObj.at(SerializeString_Parent)),
      mType(serObj.at("type")),
      mSize(serObj.at("size")),
      mGridMap(GridMap::getActiveMap()) {}

SerializedObj GridActor::serialize() const {
  SerializedObj out = createSerializedObj<GridActor>();
  out[SerializeString_Parent] = Component::serialize();
  out["type"] = mType;
  out["size"] = mSize;
  return out;
}

Vector3DInt GridActor::getCurrentPos() const { return owner().getPosition(); }

void GridActor::moveTo(const Vector3DInt& newPos) {
  unregisterMe(owner().getPosition());
  registerMe(newPos);
  owner().setPosition(newPos);
}

void GridActor::setup() { registerMe(owner().getPosition()); }

void GridActor::teardown() { unregisterMe(owner().getPosition()); }

void GridActor::registerMe(const Vector3DInt& where) {
  for (int x = 0; x < mSize.x; ++x) {
    for (int y = 0; y < mSize.y; ++y) {
      mGridMap.registerGridActorAt(where + Vector2DInt(x, y), this);
    }
  }
}

void GridActor::unregisterMe(const Vector3DInt& where) {
  for (int x = 0; x < mSize.x; ++x) {
    for (int y = 0; y < mSize.y; ++y) {
      mGridMap.unregisterGridActorAt(where + Vector2DInt(x, y), this);
    }
  }
}

const std::string& GridActor::getName() const { return owner().name(); }

GridActor::ActorType GridActor::getType() const { return mType; }
