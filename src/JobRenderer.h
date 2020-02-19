#pragma once
#include "Block.h"
#include "Camera.h"
#include "Component.h"
#include "GraphicsManager.h"
#include "GridMapHelpers.h"
#include "MiningRequestPool.h"
#include "SpriteLoader.h"

class JobRenderer : public Component {
 public:
  JobRenderer(GameObject& gObj)
      : Component(gObj),
        mGridMap(GridMap::getActiveMap()),
        mCam(Camera::get()),
        mPool(MiningRequestPool::getInstance()),
        mSprite(SpriteLoader::loadSprite(Paths::BROWN_SQUARE)) {}

  void render() override {
  }

 private:
  const GridMap& mGridMap;
  const Camera& mCam;
  MiningRequestPool& mPool;
  std::unique_ptr<Sprite> mSprite;
};