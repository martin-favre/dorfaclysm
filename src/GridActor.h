#pragma once
#include "Block.h"
#include "Component.h"
#include "GridMap.h"

class GridActor : public Component {
 public:
  enum ActorType { unset, item, dorf, building, job};
  GridActor(GameObject& gObj, ActorType type);
  GridActor(GameObject& owner, const SerializedObj& serObj);
  Vector3DInt getCurrentPos() const;
  void moveTo(const Vector3DInt& newPos);
  void setup() override;
  void teardown() override;
  const std::string& getName() const;
  ActorType getType() const;
  SerializedObj serialize() const override;
  static std::string getTypeString() {return "GridActor";}
 private:
  const ActorType mType;
  GridMap& mGridMap;
};