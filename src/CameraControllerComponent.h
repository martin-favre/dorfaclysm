#pragma once
#include "Camera.h"
#include "Component.h"
class CameraControllerComponent : public Component {
 public:
  CameraControllerComponent(GameObject& gObj);
  void render() override;

 private:
  Camera& mCam;
};