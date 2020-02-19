#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <map>

#include "Sprite.h"
#include "Vector2D.h"
#include <mutex>
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
  static void renderTexture(const Sprite&, const Vector2DInt& pos,
                            const Vector2D& scale = {1, 1},
                            const double angle = 0, bool centered = false,
                            const SDL_RendererFlip flip = SDL_FLIP_NONE);
  static void drawCircle(const Vector2D& pos, int radius);
  static void drawPoint(const Vector2D& pos);
  static void drawLine(const Vector2D& from, const Vector2D& to);
  static void drawRect(const Rect& rect);
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
  static inline unsigned int getScreenHeight() { return mScreenHeight; }
  static inline unsigned int getScreenWidth() { return mScreenWidth; }
  friend class SpriteLoader;
  friend class GridMapRenderer;
  static constexpr SDL_Color mDefaultDrawColor{0, 0, 0, 0};

 private:
  GraphicsManager();
  static bool mInitialized;
  static SDL_Window* mMainWindow;
  static SDL_Renderer* mMainRenderer;
  static SDL_Color mRenderDrawColor;
  static unsigned int mScreenWidth;
  static unsigned int mScreenHeight;
  static std::string mWindowName;
  static std::mutex mMutex;
};