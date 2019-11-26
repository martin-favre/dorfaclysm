#pragma once

#include <map>
#include <string>
#include <SDL.h>
#include <cstdint>
#include <iostream>
#include "Timer.h"
#include "Vector2DInt.h"

/* Should be slightly higher than the highest possible Scancode 
   (284 at time of writing)
*/
#define NOF_SDL_SCANCODES_BUFFER 300

/* Defines when the user presses window cross
Arbitrary value hopefully not occupied by SDL keys
*/
#define INPUT_KEY_QUIT  (NOF_SDL_SCANCODES_BUFFER-1)
/*-------------------------------------------------------
	Interface for key and mouse presses
	Uses SDL_Scancodes https://wiki.libsdl.org/SDL_Scancode as key definitions.
---------------------------------------------------------*/

class InputManager {
public:
	static void initialize();
	static void readInputs();

	/*-------------------------------------------------------
		Get the current state of the key
	---------------------------------------------------------
	@param key - which key.
	@return - true if the key is pushed down.
	---------------------------------------------------------*/
	static bool getKey(int key);

	/*-------------------------------------------------------
		Get if the key was pushed down this update
	---------------------------------------------------------
	@param key - which key.
	@return - true if the key is pushed down.
	---------------------------------------------------------*/
	static bool getKeyDown(int key);
	
	/*-------------------------------------------------------
		Get the current state of the mouse button
	---------------------------------------------------------
	@param mousebtn - which mouse button.
	@return - true if the mouse button is pushed down.
	---------------------------------------------------------*/
	static bool getMouse(int mousebtn);

	/*-------------------------------------------------------
		Get if the mouse button was pushed down this update
	---------------------------------------------------------
	@param mousebtn - which mouse button.
	@return - true if the mouse button is pushed down.
	---------------------------------------------------------*/
	static bool getMouseDown(int mousebtn);

	/*-------------------------------------------------------
		Populates evnt with the latest mouse event of that button
		returns false if this event has not happened yet.
	---------------------------------------------------------
	@param mousebtn - which mouse button.
	@param evnt - the event which is populated. If this
		event has not happened yet this is not populates.
	@return - false if the event has not happened yet.
	---------------------------------------------------------*/
	static bool getLatestMouseEvent(int mousebtn, SDL_MouseButtonEvent * evnt);

	static Vector2DInt getMousePosition();

private:
	InputManager();
	static void setKey(int key, bool value);
	static void resetKeys();
	static void addTextInput(const std::string & newText);
	static void resetTextInput();
	static void setMouse(int key, bool value, const SDL_MouseButtonEvent & evnt);
	static std::string mTextInput;
	static bool mInitialized;
	static SDL_Event mSdlEvent;
	static bool mKeyStates[NOF_SDL_SCANCODES_BUFFER];
	static bool mMouseStates[NOF_SDL_SCANCODES_BUFFER];
	static bool mMouseStatesThisFrame[NOF_SDL_SCANCODES_BUFFER];
	static SDL_MouseButtonEvent mMouseLatestEvent[NOF_SDL_SCANCODES_BUFFER];
	static bool mKeyStatesThisFrame[NOF_SDL_SCANCODES_BUFFER]; //Contains if the key was added this frame
};