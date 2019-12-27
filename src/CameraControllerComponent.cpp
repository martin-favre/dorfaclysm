#include "CameraControllerComponent.h"
#include "InputManager.h"
#include "GridMap.h"
CameraControllerComponent::CameraControllerComponent(GameObject& gObj) : Component(gObj), mCam(Camera::get()){}

void CameraControllerComponent::update()
{
  Vector2DInt movement;
  const int speedx = GridMap::tileRenderSize.x;
  const int speedy = GridMap::tileRenderSize.y;
  if(InputManager::getKeyDown(SDL_SCANCODE_W)) movement += {0,-speedy};
  if(InputManager::getKeyDown(SDL_SCANCODE_A)) movement += {-speedx,0};
  if(InputManager::getKeyDown(SDL_SCANCODE_S)) movement += {0,speedy};
  if(InputManager::getKeyDown(SDL_SCANCODE_D)) movement += {speedx,0};

  mCam.move(movement);
}