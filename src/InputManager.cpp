#include "InputManager.h"

/*-------------------------------------------------------
	Parameters.
---------------------------------------------------------*/
bool InputManager::mInitialized = false;
std::string InputManager::mTextInput = "";
bool InputManager::mKeyStates[NOF_SDL_SCANCODES_BUFFER] = {0};
bool InputManager::mMouseStates[NOF_SDL_SCANCODES_BUFFER] = {0};
bool InputManager::mMouseStatesThisFrame[NOF_SDL_SCANCODES_BUFFER] = {0};
SDL_MouseButtonEvent InputManager::mMouseLatestEvent[NOF_SDL_SCANCODES_BUFFER] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
SDL_Event InputManager::mSdlEvent;
bool InputManager::mKeyStatesThisFrame[NOF_SDL_SCANCODES_BUFFER];

/*Publics*/
void InputManager::initialize()
{
	if (!InputManager::mInitialized)
	{
		InputManager::mInitialized = true;
	}
}

void InputManager::readInputs()
{
	InputManager::resetKeys();
	InputManager::resetTextInput();

	while (SDL_PollEvent(&InputManager::mSdlEvent) != 0)
	{
		switch (InputManager::mSdlEvent.type)
		{

		//If the window cross is pressed.
		case SDL_QUIT:
			InputManager::setKey(INPUT_KEY_QUIT, true);
			break;
		//If continuous text if being added
		case SDL_TEXTINPUT:
			InputManager::addTextInput(InputManager::mSdlEvent.text.text);
			break;
		//On key up
		case SDL_KEYUP:
			InputManager::setKey(InputManager::mSdlEvent.key.keysym.scancode, false);
			break;
		//On key down
		case SDL_KEYDOWN:
			if (!InputManager::mSdlEvent.key.repeat)
			{
				InputManager::setKey(InputManager::mSdlEvent.key.keysym.scancode, true);
			}
			break;
		//On mouse up
		case SDL_MOUSEBUTTONDOWN:
			InputManager::setMouse(mSdlEvent.button.button, true, mSdlEvent.button);
			break;
		//On mouse down
		case SDL_MOUSEBUTTONUP:
			InputManager::setMouse(mSdlEvent.button.button, false, mSdlEvent.button);
			break;
		}
	}
}

bool InputManager::getKey(int key)
{
	return mKeyStates[key];
}
bool InputManager::getKeyDown(int key)
{
	return InputManager::mKeyStatesThisFrame[key];
}

bool InputManager::getMouse(int mousebtn)
{
	return InputManager::mMouseStates[mousebtn];
}

bool InputManager::getMouseDown(int mousebtn)
{
	return InputManager::mMouseStatesThisFrame[mousebtn];
}
bool InputManager::getLatestMouseEvent(int mousebtn, SDL_MouseButtonEvent *evnt)
{
	if (mMouseLatestEvent[mousebtn].type != 0)
	{
		*evnt = mMouseLatestEvent[mousebtn];
		return true;
	}
	else
	{
		return false;
	}
}

Vector2DInt InputManager::getMousePosition()
{
	Vector2DInt container;
	SDL_GetMouseState(&container.x, &container.y);
	return container;
}

/*Privates*/

void InputManager::setKey(int key, bool val)
{
	InputManager::mKeyStates[key] = val;
	if (val){
		InputManager::mKeyStatesThisFrame[key] = val;
	}
}

void InputManager::resetKeys()
{
	memset(mKeyStatesThisFrame, 0, sizeof(mKeyStatesThisFrame));
	memset(mMouseStatesThisFrame, 0, sizeof(mMouseStatesThisFrame));
}

void InputManager::addTextInput(const std::string &text)
{
	InputManager::mTextInput += text;
}

void InputManager::resetTextInput()
{
	InputManager::mTextInput = "";
}

void InputManager::setMouse(int key, bool val, const SDL_MouseButtonEvent &evnt)
{
	InputManager::mMouseStates[key] = val;
	if (val){
		InputManager::mMouseStatesThisFrame[key] = val;
	}
	InputManager::mMouseLatestEvent[key] = evnt;
}