#pragma once
#include <stack>

#include "Astar.h"
#include "Camera.h"
#include "Component.h"
#include "GraphicsManager.h"
#include "GridMap.h"
#include "InputManager.h"
#include "Rect.h"
#include "TextComponent.h"
#include "GameObject.h"
class DrawLineComponent : public Component {
 public:
  DrawLineComponent(GameObject& gObj) : Component(gObj), mCam(Camera::get()) {}
  void setup() override {
    mTextComponent = owner().getComponent<TextComponent>();
  }
  void update() override {
    if (InputManager::getMouseDown(1)) {
      mCenterPos = InputManager::getMousePosition();
      mCenterPos += mCam.getPosition();
      mCenterPos = Camera::renderPosToTilePos(mCenterPos);
      GridMap::getActiveMap().getLowestPassablePositionFrom(mCenterPos,
                                                            mCenterPos);
    }
    Vector3DInt realPos2 =
        mCam.getPosition() + InputManager::getMousePosition();
    realPos2 = Camera::renderPosToTilePos(realPos2);
    bool success = GridMap::getActiveMap().getLowestPassablePositionFrom(
        realPos2, realPos2);

    if (success) {
      Astar().getPath(mCenterPos, realPos2, GridMap::getActiveMap(), mPath);
    }
    if (mTextComponent) {
      if (success) {
        std::stringstream ss;
        ss << "From: " << mCenterPos << " to " << realPos2;
        mTextComponent->setText(ss.str());
      } else {
        std::stringstream ss;
        ss << "Unknown, From: " << mCenterPos << " to " << realPos2;
        mTextComponent->setText(ss.str());
      }
    }
  }

  void render() override {
    while (!mPath.empty()) {
      Vector3DInt pos = mPath.top();
      mPath.pop();
      GraphicsManager::setRenderDrawColor({255, 0, 0, SDL_ALPHA_OPAQUE});
      Vector3DInt camPos = mCam.getPosition();
      GraphicsManager::drawRect(
          Rect{pos.x * GridMap::tileRenderSize.x - camPos.x,
               pos.y * GridMap::tileRenderSize.y - camPos.y,
               GridMap::tileRenderSize.x, GridMap::tileRenderSize.y});
      GraphicsManager::setRenderDrawColor(GraphicsManager::mDefaultDrawColor);
    }
  }

  TextComponent* mTextComponent{nullptr};
  const Camera& mCam;
  Vector3DInt mCenterPos;
  std::stack<Vector3DInt> mPath;
};