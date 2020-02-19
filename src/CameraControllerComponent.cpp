#include "CameraControllerComponent.h"

#include "GridMap.h"
#include "InputManager.h"
CameraControllerComponent::CameraControllerComponent(GameObject& gObj)
    : Component(gObj), mCam(Camera::get()) {}

void CameraControllerComponent::render() {
  Vector3DInt movement;
  const int speedx = GridMap::tileRenderSize.x;
  const int speedy = GridMap::tileRenderSize.y;
  if (InputManager::getKeyDown(SDL_SCANCODE_W)) movement += {0, -speedy, 0};
  if (InputManager::getKeyDown(SDL_SCANCODE_A)) movement += {-speedx, 0, 0};
  if (InputManager::getKeyDown(SDL_SCANCODE_S)) movement += {0, speedy, 0};
  if (InputManager::getKeyDown(SDL_SCANCODE_D)) movement += {speedx, 0, 0};
  if(InputManager::getKeyDown(SDL_SCANCODE_NONUSBACKSLASH))
  {
    if(InputManager::getKey(SDL_SCANCODE_LSHIFT))
    {
      movement += {0, 0, -1};
    }
    else
    {
      movement += {0, 0, 1};
    }
  }
  mCam.move(movement);
}