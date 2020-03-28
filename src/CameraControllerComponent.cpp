#include "CameraControllerComponent.h"

#include "GridMap.h"
CameraControllerComponent::CameraControllerComponent(GameObject& gObj)
    : Component(gObj), mCam(Camera::get()) {}

void CameraControllerComponent::render() {
  Vector3DInt movement;
  const int speedx = Camera::tileRenderSize.x;
  const int speedy = Camera::tileRenderSize.y;
  while (InputManager::hasKeyEvents(mInputHandle)) {
    KeyEvent keyEvent = InputManager::dequeueKeyEvent(mInputHandle);
    if (!keyEvent.mKeyDown) continue;
    switch (keyEvent.mKey) {
      case SDL_SCANCODE_W:
        movement += {0, -speedy, 0};
        break;
      case SDL_SCANCODE_A:
        movement += {-speedx, 0, 0};
        break;
      case SDL_SCANCODE_S:
        movement += {0, speedy, 0};
        break;
      case SDL_SCANCODE_D:
        movement += {speedx, 0, 0};
        break;
      case SDL_SCANCODE_NONUSBACKSLASH:
        if (InputManager::getKey(SDL_SCANCODE_LSHIFT)) {
          movement += {0, 0, -1};
        } else {
          movement += {0, 0, 1};
        }
        break;
    }
  }
  if(movement != Vector3DInt::zero()){
    mCam.move(movement);
  }
}
