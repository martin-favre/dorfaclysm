#pragma once
#include "Block.h"
#include "Camera.h"
#include "Component.h"
#include "GridMapHelpers.h"
#include "SpriteLoader.h"
#include "GraphicsManager.h"
class JobRenderer : public Component {
  public:
  JobRenderer(GameObject& gObj)
      : Component(gObj),
        mGridMap(GridMap::getActiveMap()),
        mCam(Camera::get()),
        mSprite(SpriteLoader::loadSprite(Paths::BROWN_SQUARE))

  {}

  void renderJob(const Vector3DInt& tilePos) {
    const Block& block = mGridMap.getBlockAt(tilePos);
    if (block.getJob() != jobTypeUnset) {
      Vector3DInt renderPos = mCam.tilePosToRenderPos(tilePos);
      renderPos-=mCam.getPosition();
      GraphicsManager::renderTexture(*mSprite, renderPos);
    }
  }
  void render() override {
    std::function<void(const Vector3DInt&)> foo =
        [this](const Vector3DInt& pos) { this->renderJob(pos); };
    GridMapHelpers::doToEachBlockInScreen(mGridMap, mCam, foo);
  }

 private:
  const GridMap& mGridMap;
  const Camera& mCam;
  std::unique_ptr<Sprite> mSprite;
};