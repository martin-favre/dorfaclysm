#include "FpsCounter.h"

#include "GameObject.h"
#include "Paths.h"
#include "TextComponent.h"

FpsCounter::FpsCounter(GameObject& gObj) : Component(gObj) { mTimer.start(); }

void FpsCounter::setup() { mText = owner().getComponent<TextComponent>(); }

void FpsCounter::update() {
  if (mText) {
    auto ms = mTimer.getElapsedMilliseconds();
    std::string text = std::to_string(ms);
    mText->setText(text + "ms/frame");
    mTimer.start();
  }
}