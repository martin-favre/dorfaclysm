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
  enum Mode {mine, place};
  void renderText();
  void handleClick();
  TextComponent* mTextComponent{nullptr};
  Mode mMode{mine};
};