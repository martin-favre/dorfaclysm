#pragma once 
#include "Component.h"
#include "Serializer.h"

class MineBlockComponent : public Component{
  public:
  MineBlockComponent(GameObject& gObj);
  MineBlockComponent(GameObject& gObj, const SerializedObj& serObj);
  void setup() override;
  SerializedObj serialize() const override;
  static std::string getTypeString() { return "MineBlockComponent";}
  private:
  void onJobComplete();
};