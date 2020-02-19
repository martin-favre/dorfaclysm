#pragma once 
#include "Component.h"

class MineBlockComponent : public Component{
  public:
  MineBlockComponent(GameObject& gObj);
  void setup() override;
  private:
  void onJobComplete();
};