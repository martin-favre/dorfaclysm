#pragma once
#include "Component.h"
#include "Camera.h"
class CameraControllerComponent : public Component
{
  public:
  CameraControllerComponent(GameObject& gObj);
  void update()override;
  private:
  Camera& mCam;
};