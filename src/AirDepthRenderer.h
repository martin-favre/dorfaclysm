#pragma once
#include "Component.h"
#include "Vector3DInt.h"
class GridMap;
class Camera;
class AirDepthRenderer : public Component
{
  public:
  AirDepthRenderer(GameObject& gObj);
  void render() override;
  private:
  void renderBlock(Vector3DInt pos);
  const GridMap& mGridMap;
  const Camera& mCam;
};