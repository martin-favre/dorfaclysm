#pragma once

#include <SDL.h>

#include <list>
#include <mutex>
#include <queue>
#include <string>

#include "Vector2DInt.h"
/* Should be slightly higher than the highest possible Scancode
   (284 at time of writing)
*/
#define NOF_SDL_SCANCODES_BUFFER 300

/* Defines when the user presses window cross
Arbitrary value hopefully not occupied by SDL keys
*/
#define INPUT_KEY_QUIT (NOF_SDL_SCANCODES_BUFFER - 1)
/*-------------------------------------------------------
        Interface for key and mouse presses
        Uses SDL_Scancodes https://wiki.libsdl.org/SDL_Scancode as key
definitions.

Usage:

QueueHandle myHandle;
while (InputManager::hasKeyEvents(myHandle)) {
  KeyEvent keyEvent = InputManager::dequeueKeyEvent(myHandle);
  switch (keyEvent.mKey) {
    ??
  }
}

---------------------------------------------------------*/

class KeyEvent {
 public:
  KeyEvent(int key, bool keyDown);
  const int mKey;
  const bool mKeyDown;
};

class MouseEvent {
 public:
  MouseEvent(const Vector2DInt& pos, int button, bool buttonDown);
  const Vector2DInt mPos;
  const int mButton;
  const bool mButtonDown;
};

class InputManager;
class QueueHandle {
 public:
  QueueHandle();
  ~QueueHandle();
  friend class InputManager;

 private:
  std::queue<MouseEvent> mMouseEvents;
  std::queue<KeyEvent> mKeyEvents;
};

class InputManager {
 public:
  static void initialize();
  static void readInputs();

  static bool hasMouseEvents(const QueueHandle& handle);
  static bool hasKeyEvents(const QueueHandle& handle);

  static MouseEvent dequeueMouseEvent(QueueHandle& handle);
  static KeyEvent dequeueKeyEvent(QueueHandle& handle);

  static void clearQueues(QueueHandle& handle);

  /*-------------------------------------------------------
          Get the current state of the key
  ---------------------------------------------------------
  @param key - which key.
  @return - true if the key is pushed down.
  ---------------------------------------------------------*/
  static bool getKey(int key);

  /*-------------------------------------------------------
          Get the current state of the mouse button
  ---------------------------------------------------------
  @param mousebtn - which mouse button.
  @return - true if the mouse button is pushed down.
  ---------------------------------------------------------*/
  static bool getMouse(int mousebtn);

  static Vector2DInt getMousePosition();
  friend class QueueHandle;

 private:
  static void registerQueues(QueueHandle* handle);
  static void unRegisterQueues(QueueHandle* handle);
  InputManager();
  static void setKey(int key, bool value);
  static void addTextInput(const std::string& newText);
  static void resetTextInput();
  static void setMouse(int key, bool value, const SDL_MouseButtonEvent& evnt);
  static std::string mTextInput;
  static bool mInitialized;
  static SDL_Event mSdlEvent;
  static bool mKeyStates[NOF_SDL_SCANCODES_BUFFER];
  static bool mMouseStates[NOF_SDL_SCANCODES_BUFFER];
  static std::mutex mMutex;
  static std::list<QueueHandle*> mQueueHandles;
};