#pragma once
#include "Component.h"

class GridMap;
class Camera;
class AirDepthRenderer : public Component
{
  public:
  AirDepthRenderer(GameObject& gObj);
  void render() override;
  private:
  const GridMap& mGridMap;
  const Camera& mCam;
};