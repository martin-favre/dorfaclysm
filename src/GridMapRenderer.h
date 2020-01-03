#pragma once
#include <SDL.h>

#include "Component.h"
#include "GridMap.h"
#include "Sprite.h"
class GridMapRenderer : public Component {
 public:
  GridMapRenderer(GameObject& g);

  void setup() override;
  void update() override;
  void render() override;

 private:
  void prepareViewedArea();
  const GridMap& mActiveGridMap;
};