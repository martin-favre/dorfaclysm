#pragma once
#include "Block.h"
#include "Component.h"
#include "GridMap.h"
#include "Vector2DInt.h"
#include "Vector3DInt.h"

class GridActor : public Component {
 public:
  enum ActorType { unset, item, dorf, building, job };
  GridActor(GameObject& gObj, ActorType type, const Vector2DInt& size = {1, 1});
  GridActor(GameObject& owner, const SerializedObj& serObj);
  Vector3DInt getCurrentPos() const;
  void moveTo(const Vector3DInt& newPos);
  void setup() override;
  void teardown() override;
  const std::string& getName() const;
  ActorType getType() const;
  SerializedObj serialize() const override;
  static std::string getTypeString() { return "GridActor"; }

 private:
  void registerMe(const Vector3DInt& where);
  void unregisterMe(const Vector3DInt& where);
  const ActorType mType;
  const Vector2DInt mSize{1, 1};
  GridMap& mGridMap;
};