#pragma once
#include "Component.h"
#include "TextComponent.h"
class PlayerControllerComponent : public Component
{
  public:
  PlayerControllerComponent(GameObject& gObj) : Component(gObj){}
  void setup()override;
  void update()override;
  private:
  TextComponent* mTextComponent{nullptr};
};