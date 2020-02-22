#pragma once
#include "Camera.h"
#include "Component.h"
#include "InputManager.h"

class CameraControllerComponent : public Component {
 public:
  CameraControllerComponent(GameObject& gObj);
  void render() override;

 private:
  Camera& mCam;
  QueueHandle mInputHandle;
};