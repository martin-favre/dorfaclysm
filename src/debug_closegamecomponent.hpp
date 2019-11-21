#pragma once
#include "Engine.h"
#include "InputManager.h"

class GameObject;
struct Debug_CloseGameComponent : public Component {
	Debug_CloseGameComponent(GameObject*owner) :Component(owner) {}
	inline void update() final override {
		if (InputManager::get_key_down(INPUT_KEY_QUIT) ||
		        InputManager::get_key_down(SDL_SCANCODE_Q)) {
			Engine::stop();
		}
	}
};