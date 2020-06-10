#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Serializer.h"
#include "TextComponent.h"
#include "InputManager.h"
#include <map>
#include <string>
class PlayerControllerComponent : public Component
{
  public:
  PlayerControllerComponent(GameObject& gObj);
  PlayerControllerComponent(GameObject& gObj, const SerializedObj& serObj);
  SerializedObj serialize() const override; 
  void setup()override;
  void update()override;
  void render()override;
  static std::string getTypeString();
  private:
  enum Mode {mine, place, clear};
  void renderText();
  void handleClick();
  TextComponent* mTextComponent{nullptr};
  Mode mMode{mine};
  QueueHandle mInputHandle;

  static const std::map<int, PlayerControllerComponent::Mode> mKeyToMode;
};