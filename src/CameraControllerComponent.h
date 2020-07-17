#pragma once
#include "Camera.h"
#include "Component.h"
#include "InputManager.h"
#include "Serializer.h"

class CameraControllerComponent : public Component {
 public:
  CameraControllerComponent(GameObject& gObj);
  CameraControllerComponent(GameObject& gObj, const SerializedObj& serObj);
  void render() override;
  SerializedObj serialize() const override;
  static std::string getTypeString();
 private:
  Camera& mCam;
  QueueHandle mInputHandle;
};