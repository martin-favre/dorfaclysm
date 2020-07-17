#include "ShowPositionComponent.h"

#include "Camera.h"
#include "Component.h"
#include "GameObject.h"
#include "Serializer.h"
#include "TextComponent.h"
ShowPositionComponent::ShowPositionComponent(GameObject& owner)
    : Component(owner) {}

ShowPositionComponent::ShowPositionComponent(GameObject& owner,
                                             const SerializedObj& serObj)
    : Component(owner, serObj["parent"]) {}
SerializedObj ShowPositionComponent::serialize() const {
  SerializedObj out = createSerializedObj<ShowPositionComponent>();
  out["parent"] = Component::serialize();
  return out;
}
std::string ShowPositionComponent::getTypeString() { return "ShowPositionComponent"; }
void ShowPositionComponent::setup() {
  mText = owner().getComponent<TextComponent>();
}
void ShowPositionComponent::update() {
  if (mText) {
    std::stringstream ss;
    ss << Camera::get().getPosition();
    std::string stringText = ss.str();
    if (stringText != mText->getText()) mText->setText(stringText);
  }
}
