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
	static void read_inputs();

	/*-------------------------------------------------------
		Get the current state of the key
	---------------------------------------------------------
	@param key - which key.
	@return - true if the key is pushed down.
	---------------------------------------------------------*/
	static bool get_key(int key);

	/*-------------------------------------------------------
		Get if the key was pushed down this update
	---------------------------------------------------------
	@param key - which key.
	@return - true if the key is pushed down.
	---------------------------------------------------------*/
	static bool get_key_down(int key);
	
	/*-------------------------------------------------------
		Get the current state of the mouse button
	---------------------------------------------------------
	@param mousebtn - which mouse button.
	@return - true if the mouse button is pushed down.
	---------------------------------------------------------*/
	static bool get_mouse(int mousebtn);

	/*-------------------------------------------------------
		Get if the mouse button was pushed down this update
	---------------------------------------------------------
	@param mousebtn - which mouse button.
	@return - true if the mouse button is pushed down.
	---------------------------------------------------------*/
	static bool get_mouse_down(int mousebtn);

	/*-------------------------------------------------------
		Populates evnt with the latest mouse event of that button
		returns false if this event has not happened yet.
	---------------------------------------------------------
	@param mousebtn - which mouse button.
	@param evnt - the event which is populated. If this
		event has not happened yet this is not populates.
	@return - false if the event has not happened yet.
	---------------------------------------------------------*/
	static bool get_latest_mouse_event(int mousebtn, SDL_MouseButtonEvent * evnt);

	static Vector2DInt get_mouse_position();

private:
	InputManager();
	static void set_key(int key, bool value);
	static void reset_keys();
	static void add_text_input(const std::string & new_text);
	static void reset_text_input();
	static void set_mouse(int key, bool value, const SDL_MouseButtonEvent & evnt);
	static std::string m_text_input;
	static bool mInitialized;
	static bool m_mouse_state;
	static SDL_Event m_sdl_event;
	static bool m_key_states[NOF_SDL_SCANCODES_BUFFER];
	static bool m_mouse_states[NOF_SDL_SCANCODES_BUFFER];
	static bool m_mouse_states_current[NOF_SDL_SCANCODES_BUFFER];
	static SDL_MouseButtonEvent m_mouse_latest_event[NOF_SDL_SCANCODES_BUFFER];
	static bool m_key_states_current[NOF_SDL_SCANCODES_BUFFER]; //Contains if the key was added this frame
};