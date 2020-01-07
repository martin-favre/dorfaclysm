#pragma once

#include "Component.h"
class TextComponent;
class ShowPositionComponent : public Component
{
  public:
  ShowPositionComponent(GameObject& owner);
  void setup() override;
  void update() override;
  private:
  TextComponent* mText{nullptr};
};