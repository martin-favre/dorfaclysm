#pragma once
#include "Engine.h"
#include "InputManager.h"

class GameObject;
struct Debug_CloseGameComponent : public Component {
  Debug_CloseGameComponent(GameObject& owner) : Component(owner) {}
  inline void render() final override {
    if (InputManager::getKeyDown(INPUT_KEY_QUIT) ||
        InputManager::getKeyDown(SDL_SCANCODE_Q)) {
      Engine::stop();
    }
  }
};