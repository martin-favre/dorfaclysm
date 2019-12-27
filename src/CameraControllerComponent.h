#pragma once
#include "Camera.h"
#include "Component.h"
class CameraControllerComponent : public Component {
 public:
  CameraControllerComponent(GameObject& gObj);
  void update() override;

 private:
  Camera& mCam;
};