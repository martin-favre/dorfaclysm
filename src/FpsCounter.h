#pragma once
#include "Component.h"
#include "Timer.h"
class TextComponent;

class FpsCounter : public Component {
 public:
  FpsCounter(GameObject& gObj);
  void setup() override;
  void update() override;

 private:
  Timer mTimer;
  TextComponent* mText{nullptr};
};