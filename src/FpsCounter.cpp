#include "FpsCounter.h"

#include "Component.h"
#include "GameObject.h"
#include "Paths.h"
#include "Serializer.h"
#include "TextComponent.h"

FpsCounter::FpsCounter(GameObject& gObj) : Component(gObj) {
  mUpdateTimer.start();
  mRenderTimer.start();
}
FpsCounter::FpsCounter(GameObject& gObj, const SerializedObj& serObj)
    : Component(gObj, serObj["parent"]) {}
SerializedObj FpsCounter::serialize() const {
  SerializedObj out = createSerializedObj<FpsCounter>();
  out["parent"] = Component::serialize();
  return out;
}
void FpsCounter::setup() { mText = owner().getComponent<TextComponent>(); }
std::string FpsCounter::getTypeString() { return "FpsCounter"; }
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
