#pragma once
#include "Block.h"
#include "Component.h"
#include "GridMap.h"

class GridActor : public Component {
 public:
  GridActor(GameObject& gObj);

  const Vector3DInt& getCurrentPos() const;

  void moveTo(const Vector3DInt& newPos);

  void setup() override;

  void teardown() override;

  const std::string& getName() const;

 private:
  GridMap& mGridMap;
};