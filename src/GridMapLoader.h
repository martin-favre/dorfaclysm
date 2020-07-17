#pragma once
#include "Component.h"

class GridMapLoader : public Component{
  public:
  GridMapLoader(GameObject& owner);
  GridMapLoader(GameObject& owner, const SerializedObj& serObj);
  SerializedObj serialize() const override;
  static std::string getTypeString();
};