#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "InputManager.h"
#include <map>
class PlayerControllerComponent : public Component
{
  public:
  PlayerControllerComponent(GameObject& gObj);
  void setup()override;
  void update()override;
  void render()override;
  private:
  enum Mode {mine, place, clear};
  void renderText();
  void handleClick();
  TextComponent* mTextComponent{nullptr};
  Mode mMode{mine};
  QueueHandle mInputHandle;

  static const std::map<int, PlayerControllerComponent::Mode> mKeyToMode;
};