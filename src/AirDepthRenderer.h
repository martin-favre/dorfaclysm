#pragma once
#include "Component.h"
#include "Serializer.h"
#include "Vector3DInt.h"
class GridMap;
class Camera;
class AirDepthRenderer : public Component
{
  public:
  AirDepthRenderer(GameObject& gObj);
  AirDepthRenderer(GameObject& gObj, const SerializedObj& serObj);
  SerializedObj serialize() const override;
  void render() override;
  static std::string getTypeString();
  private:
  void renderBlock(Vector3DInt pos);
  const GridMap& mGridMap;
  const Camera& mCam;
};