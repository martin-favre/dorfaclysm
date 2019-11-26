#pragma once


#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Vector2D.h"
#include "Sprite.h"

/*-------------------------------------------------------
	Interface for presenting images to the screen.
	Owns prepared textures, loaded from files.
	Users simply asks to present it.
---------------------------------------------------------*/

class GraphicsManager {
public:
	static void initialize();
	static void teardown();
	static void prepareRendering();
	static void renderTexture(const Sprite &,
	                           const Vector2D & pos,
	                           const Vector2D & scale,
	                           const double angle,
	                           bool centered,
	                           const SDL_RendererFlip flip);
	static void drawCircle(const Vector2D & pos, int radius);
	static void drawPoint(const Vector2D& pos);
	static void drawLine(const Vector2D& from, const Vector2D& to);
	static void executeRendering();
	static void setRenderDrawColor(const SDL_Color& color);
	/*-------------------------------------------------------
		Converts the surface to an SDL_Texture.
		Note that GraphicsManager does not own the returned
		texture and it must be freed accordingly.
	---------------------------------------------------------
	@param surf - The surface to convert to a texture.
	@return - pointer to the created texture.
	---------------------------------------------------------*/
	static unsigned int getScreenHeight() {return mScreenHeight;}
	static unsigned int getScreenWidth() {return mScreenWidth;}
	friend class SpriteLoader;

private:
	GraphicsManager();
	static bool mInitialized;
	static SDL_Window * mMainWindow;
	static SDL_Surface * mMainSurface;
	static SDL_Renderer * mMainRenderer;
	static SDL_Color mRenderDrawColor;
	static unsigned int mScreenWidth;
	static unsigned int mScreenHeight;
	static std::string mWindowName;
};