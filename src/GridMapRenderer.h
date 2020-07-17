#pragma once
#include <SDL.h>

#include "Component.h"
#include "GridMap.h"
#include "Serializer.h"
#include "Sprite.h"
class GridMapRenderer : public Component {
 public:
  GridMapRenderer(GameObject& g);
  GridMapRenderer(GameObject& g, const SerializedObj& serObj);
  void setup() override;
  void update() override;
  void render() override;
  static std::string getTypeString();
  SerializedObj serialize() const override;
 private:
  void prepareViewedArea();
  const GridMap& mActiveGridMap;
};