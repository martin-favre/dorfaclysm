#include "InputManager.h"

#include "Logging.h"
/*-------------------------------------------------------
        Parameters.
---------------------------------------------------------*/
bool InputManager::mInitialized = false;
std::string InputManager::mTextInput = "";
bool InputManager::mKeyStates[NOF_SDL_SCANCODES_BUFFER] = {0};
bool InputManager::mMouseStates[NOF_SDL_SCANCODES_BUFFER] = {0};
SDL_Event InputManager::mSdlEvent;
std::mutex InputManager::mMutex;
std::list<QueueHandle*> InputManager::mQueueHandles;

QueueHandle::QueueHandle() { InputManager::registerQueues(this); }
QueueHandle::~QueueHandle() { InputManager::unRegisterQueues(this); }

KeyEvent::KeyEvent(int key, bool keyDown) : mKey(key), mKeyDown(keyDown) {}
MouseEvent::MouseEvent(const Vector2DInt& pos, int button, bool buttonDown)
    : mPos(pos), mButton(button), mButtonDown(buttonDown) {}

/*Publics*/
void InputManager::initialize() {
  if (!InputManager::mInitialized) {
    InputManager::mInitialized = true;
  }
}

void InputManager::registerQueues(QueueHandle* handle) {
  std::scoped_lock lock(mMutex);
  mQueueHandles.emplace_back(handle);
}
void InputManager::unRegisterQueues(QueueHandle* handle) {
  std::scoped_lock lock(mMutex);
  mQueueHandles.erase(
      std::remove(mQueueHandles.begin(), mQueueHandles.end(), handle),
      mQueueHandles.end());
}

bool InputManager::hasMouseEvents(const QueueHandle& handle) {
  std::scoped_lock lock(mMutex);
  return !handle.mMouseEvents.empty();
}
bool InputManager::hasKeyEvents(const QueueHandle& handle) {
  std::scoped_lock lock(mMutex);
  return !handle.mKeyEvents.empty();
}
MouseEvent InputManager::dequeueMouseEvent(QueueHandle& handle) {
  std::scoped_lock lock(mMutex);
  MouseEvent out = handle.mMouseEvents.front();
  handle.mMouseEvents.pop();
  return out;
}
KeyEvent InputManager::dequeueKeyEvent(QueueHandle& handle) {
  std::scoped_lock lock(mMutex);
  KeyEvent out = handle.mKeyEvents.front();
  handle.mKeyEvents.pop();
  return out;
}

void InputManager::clearQueues(QueueHandle& handle) {
  std::scoped_lock lock(mMutex);
  while (!handle.mMouseEvents.empty()) handle.mMouseEvents.pop();
  while (!handle.mKeyEvents.empty()) handle.mKeyEvents.pop();
}

void InputManager::readInputs() {
  std::scoped_lock lock(mMutex);
  InputManager::resetTextInput();

  while (SDL_PollEvent(&InputManager::mSdlEvent) != 0) {
    switch (InputManager::mSdlEvent.type) {
      // If the window cross is pressed.
      case SDL_QUIT:
        InputManager::setKey(INPUT_KEY_QUIT, true);
        break;
      // If continuous text if being added
      case SDL_TEXTINPUT:
        InputManager::addTextInput(InputManager::mSdlEvent.text.text);
        break;
      case SDL_KEYUP:
        InputManager::setKey(InputManager::mSdlEvent.key.keysym.scancode,
                             false);
        break;
      case SDL_KEYDOWN:
        if (!InputManager::mSdlEvent.key.repeat) {
          InputManager::setKey(InputManager::mSdlEvent.key.keysym.scancode,
                               true);
        }
        break;
      case SDL_MOUSEBUTTONDOWN: {
        InputManager::setMouse(mSdlEvent.button.button, true, mSdlEvent.button);
        Vector2DInt container;
        SDL_GetMouseState(&container.x, &container.y);
        break;
      }
      case SDL_MOUSEBUTTONUP:
        InputManager::setMouse(mSdlEvent.button.button, false,
                               mSdlEvent.button);
        break;
    }
  }
}

bool InputManager::getKey(int key) {
  std::scoped_lock lock(mMutex);
  return mKeyStates[key];
}

bool InputManager::getMouse(int mousebtn) {
  std::scoped_lock lock(mMutex);
  return InputManager::mMouseStates[mousebtn];
}

Vector2DInt InputManager::getMousePosition() {
  Vector2DInt container;
  SDL_GetMouseState(&container.x, &container.y);
  return container;
}

/*Privates*/

void InputManager::setKey(int key, bool val) {
  InputManager::mKeyStates[key] = val;
  if (val) {
    for (auto& handle : mQueueHandles) {
      handle->mKeyEvents.emplace(key, val);
    }
  }
}

void InputManager::addTextInput(const std::string& text) {
  InputManager::mTextInput += text;
}

void InputManager::resetTextInput() { InputManager::mTextInput = ""; }

void InputManager::setMouse(int key, bool val,
                            const SDL_MouseButtonEvent& evnt) {
  InputManager::mMouseStates[key] = val;
  if (val) {
    for (auto& handle : mQueueHandles) {
      Vector2DInt pos{evnt.x, evnt.y};
      handle->mMouseEvents.emplace(pos, key, val);
    }
  }
}