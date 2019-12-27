#pragma once
#include "Component.h"
#include "GridMap.h"
#include "Sprite.h"
class GridMapRenderer : public Component {
 public:
  GridMapRenderer(GameObject& g);

  void render() override;

 private:
  void renderTile(const Tile& tile, const Vector2DInt& pos);
  const GridMap& mActiveGridMap;
  const std::unique_ptr<Sprite> grassSprite;
  const std::unique_ptr<Sprite> dirtSprite;
};