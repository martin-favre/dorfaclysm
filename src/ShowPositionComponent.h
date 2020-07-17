#pragma once

#include "Component.h"
#include "Serializer.h"
class TextComponent;
class ShowPositionComponent : public Component {
 public:
  ShowPositionComponent(GameObject& owner);
  ShowPositionComponent(GameObject& owner, const SerializedObj&);
  SerializedObj serialize() const override;
  void setup() override;
  void update() override;
  static std::string getTypeString();

 private:
  TextComponent* mText{nullptr};
};