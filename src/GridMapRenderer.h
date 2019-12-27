#pragma once
#include "Component.h"
#include "GridMap.h"
#include "Sprite.h"
#include <SDL.h>
class GridMapRenderer : public Component {
 public:
  GridMapRenderer(GameObject& g);

  void setup() override;
  void update() override;
  void render() override;

 private:
  Sprite& getSpriteFromTile(const Tile& tile);
  void prepareViewedArea();
  const GridMap& mActiveGridMap;
  Vector2DInt mPreviousCameraPosition;
  const std::unique_ptr<Sprite> mGrassSprite;
  const std::unique_ptr<Sprite> mDirtSprite;
  SDL_Texture * mActiveTexture{nullptr};
};