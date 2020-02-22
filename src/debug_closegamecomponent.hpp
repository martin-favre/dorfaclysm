#pragma once
#include "Engine.h"
#include "InputManager.h"

class GameObject;
struct Debug_CloseGameComponent : public Component {
  Debug_CloseGameComponent(GameObject& owner) : Component(owner) {}
  inline void update() final override {
    while (InputManager::hasKeyEvents(mInputHandle)) {
      KeyEvent keyEvent = InputManager::dequeueKeyEvent(mInputHandle);
      if (!keyEvent.mKeyDown) continue;
      switch (keyEvent.mKey) {
        case INPUT_KEY_QUIT:
        case SDL_SCANCODE_Q:
          Engine::stop();
          break;
        default:
          break;
      }
    }
  }

 private:
  QueueHandle mInputHandle;
};