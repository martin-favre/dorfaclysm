#include "FpsCounter.h"

#include "GameObject.h"
#include "Paths.h"
#include "TextComponent.h"

FpsCounter::FpsCounter(GameObject& gObj) : Component(gObj) { mUpdateTimer.start();
  mRenderTimer.start();
 }

void FpsCounter::setup() { mText = owner().getComponent<TextComponent>(); }

void FpsCounter::update() {
  if (mText) {
    auto ms = mUpdateTimer.getElapsedMilliseconds();
    std::string updateText = std::to_string(ms);
    std::string renderText = std::to_string(mMsPerRender);
    mText->setText(updateText + "ms/update\n" + renderText + "ms/render\n");
    mUpdateTimer.start();
  }
}

void FpsCounter::render() {
  mMsPerRender = mRenderTimer.getElapsedMilliseconds();
  mRenderTimer.start();
}
