#include "GraphicsManager.h"

#include "Helpers.h"
#include "Logging.h"
#include "SpriteLoader.h"
#include "Vector2DInt.h"
/* Parameters */

bool GraphicsManager::mInitialized = false;
SDL_Window* GraphicsManager::mMainWindow = nullptr;
SDL_Renderer* GraphicsManager::mMainRenderer = nullptr;
unsigned int GraphicsManager::mScreenWidth = 1700;
unsigned int GraphicsManager::mScreenHeight = 800;
std::string GraphicsManager::mWindowName = "Let's go bois";
SDL_Color GraphicsManager::mRenderDrawColor = {0, 0, 0, 1};
std::mutex GraphicsManager::mMutex;
/* Public Routines */

void GraphicsManager::initialize() {
  ASSERT(!GraphicsManager::mInitialized,
         "You cannot initialize GraphicsManager twice!");
  int ok = SDL_Init(SDL_INIT_VIDEO);
  ASSERT(ok >= 0,
         "SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
                   "0"))  // 0 = nearest pixel sampling
  {
    LOGL("Warning: Linear texture filtering not enabled!", Logging::warning);
  }

  GraphicsManager::mMainWindow = SDL_CreateWindow(
      GraphicsManager::mWindowName.c_str(), SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  ASSERT(
      GraphicsManager::mMainWindow != NULL,
      "Window could not be created! SDL_Error:" + std::string(SDL_GetError()));

  GraphicsManager::mMainRenderer =
      SDL_CreateRenderer(GraphicsManager::mMainWindow, -1,
                         SDL_RENDERER_SOFTWARE); 
                         // SDL_RENDERER_ACCELERATED caused issues (black sprites) depending on computer

  ASSERT(GraphicsManager::mMainRenderer != NULL,
         "Renderer could not be created! SDL Error: " +
             std::string(SDL_GetError()));
  ok = SDL_SetRenderDrawBlendMode(GraphicsManager::mMainRenderer,
                                  SDL_BLENDMODE_BLEND);
  ASSERT(ok >= 0, "Could not SDL_SetRenderDrawBlendMode: " +
                      std::string(SDL_GetError()));

  setRenderDrawColor(mDefaultDrawColor);

  int img_flags = IMG_INIT_PNG;
  ok = IMG_Init(img_flags) & img_flags;

  ASSERT(ok, "SDL_image could not initialize! SDL_image Error: " +
                 std::string(IMG_GetError()));

  ok = TTF_Init();
  ASSERT(ok != -1, "SDL_ttf could not initialize! SDL_ttf Error: " +
                       std::string(TTF_GetError()));
  GraphicsManager::mInitialized = true;
  LOGL("Finished initialing GraphicsManager", Logging::info);
}

void GraphicsManager::teardown() {
  SpriteLoader::teardown();
  SDL_DestroyRenderer(GraphicsManager::mMainRenderer);
  SDL_DestroyWindow(GraphicsManager::mMainWindow);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  mInitialized = false;
  LOGL("Finished teardown GraphicsManager", Logging::info);
}

void GraphicsManager::prepareRendering() {
  setRenderDrawColor(mDefaultDrawColor);
  std::scoped_lock lock(mMutex);
  SDL_RenderClear(GraphicsManager::mMainRenderer);
}

void GraphicsManager::renderTexture(const Sprite& sprite,
                                    const Vector2DInt& pos,
                                    const Vector2D& scale, const double angle,
                                    bool centered,
                                    const SDL_RendererFlip flip) {
  SDL_Texture* texture = sprite.getSdlTexture();
  ASSERT(texture != nullptr, "Trying to render null texture");

  const SDL_Rect& spriteDimensions = sprite.getRect().getSdlRect();

  double posx = 0;
  double posy = 0;
  const double scalex = spriteDimensions.w * scale.x;
  const double scaley = spriteDimensions.h * scale.y;
  if (centered) {
    posx = round(pos.x - scalex / 2);
    posy = round(pos.y - scaley / 2);
  } else {
    posx = pos.x;
    posy = pos.y;
  }
  const SDL_Rect dstrect = {static_cast<int>(posx), static_cast<int>(posy),
                            static_cast<int>(scalex), static_cast<int>(scaley)};
  std::scoped_lock lock(mMutex);
  SDL_RenderCopyEx(
      GraphicsManager::mMainRenderer,  // SDL_Renderer*          renderer
      texture,                         // SDL_Texture*           texture
      &sprite.getRect().getSdlRect(),  // const SDL_Rect*        srcrect.
                                       // selects a subpart of the texture.
      &dstrect,                        // const SDL_Rect*        dstrect
      angle,                           // const double           angle
      NULL,                            // const SDL_Point*       center
      flip);                           // const SDL_RendererFlip flip
}

void GraphicsManager::drawCircle(const Vector2D& pos, int radius) {
  /* Stolen by
   * https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
   */
  int x = radius - 1;
  int y = 0;
  int tx = 1;
  int ty = 1;
  int err = tx - (radius << 1);  // shifting bits left by 1 effectively
                                 // doubles the value. == tx - diameter
  int posx = Helpers::roundToInt(pos.x);
  int posy = Helpers::roundToInt(pos.y);
  std::scoped_lock lock(mMutex);
  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(mMainRenderer, posx + x, posy - y);
    SDL_RenderDrawPoint(mMainRenderer, posx + x, posy + y);
    SDL_RenderDrawPoint(mMainRenderer, posx - x, posy - y);
    SDL_RenderDrawPoint(mMainRenderer, posx - x, posy + y);
    SDL_RenderDrawPoint(mMainRenderer, posx + y, posy - x);
    SDL_RenderDrawPoint(mMainRenderer, posx + y, posy + x);
    SDL_RenderDrawPoint(mMainRenderer, posx - y, posy - x);
    SDL_RenderDrawPoint(mMainRenderer, posx - y, posy + x);

    if (err <= 0) {
      y++;
      err += ty;
      ty += 2;
    }
    if (err > 0) {
      x--;
      tx += 2;
      err += tx - (radius << 1);
    }
  }
}

void GraphicsManager::drawPoint(const Vector2D& pos) {
  int x = Helpers::roundToInt(pos.x);
  int y = Helpers::roundToInt(pos.y);
  std::scoped_lock lock(mMutex);
  int success = SDL_RenderDrawPoint(mMainRenderer, x, y);
  ASSERT(success == 0, SDL_GetError());
}

void GraphicsManager::drawLine(const Vector2D& from, const Vector2D& to) {
  int from_x = (int)round(from.x);
  int from_y = (int)round(from.y);
  int to_x = (int)round(to.x);
  int to_y = (int)round(to.y);
  std::scoped_lock lock(mMutex);
  int success = SDL_RenderDrawLine(mMainRenderer, from_x, from_y, to_x, to_y);
  ASSERT(success == 0, SDL_GetError());
}

void GraphicsManager::drawRect(const Rect& rect) {
  std::scoped_lock lock(mMutex);
  SDL_RenderFillRect(mMainRenderer, &rect.getSdlRect());
}

void GraphicsManager::executeRendering() {
  std::scoped_lock lock(mMutex);
  SDL_RenderPresent(GraphicsManager::mMainRenderer);
}

void GraphicsManager::setRenderDrawColor(const SDL_Color& color) {
  std::scoped_lock lock(mMutex);
  SDL_SetRenderDrawColor(GraphicsManager::mMainRenderer, color.r, color.g,
                         color.b, color.a);
}
