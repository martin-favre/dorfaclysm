#include "GridMapRenderer.h"

#include "Camera.h"
#include "GraphicsManager.h"
#include "Helpers.h"
#include "Logging.h"
#include "Paths.h"
#include "SpriteLoader.h"
#include "Tile.h"
#include "Vector2DInt.h"

void GridMapRenderer::setup() { prepareViewedArea(); }

/**
 * Rendering the whole map, sprite for sprite, is performance expensive.
 * Instead pre-generate a texture.
 * Generating a texture for the whole map is memory expensive.
 * So generate just the current screen.
 */
void GridMapRenderer::prepareViewedArea() {
  Vector2DInt cameraPos = Camera::get().getPosition();
  Vector2DInt cameraScale{1, 1};
  Vector2DInt tileSize{GridMap::tileRenderSize};
  const Vector2DInt mapSize{mActiveGridMap.getSize()};
  tileSize.x *= cameraScale.x;
  tileSize.y *= cameraScale.y;
  Vector2DInt screenSize{GraphicsManager::getScreenWidth(),
                         GraphicsManager::getScreenHeight()};

  int numberOfTilesToRenderX = screenSize.x / tileSize.x + 1;
  int numberOfTilesToRenderY = screenSize.y / tileSize.y + 1;

  SDL_Renderer* renderer = GraphicsManager::mMainRenderer;

  if (mActiveTexture == nullptr) {
    mActiveTexture = SDL_CreateTexture(
        renderer, GraphicsManager::mMainSurface->format->format,
        SDL_TEXTUREACCESS_TARGET, screenSize.x, screenSize.y);
    ASSERT(mActiveTexture,
           "could not create texture " + std::string(SDL_GetError()));
  }
  {
    int success =
        SDL_SetRenderTarget(GraphicsManager::mMainRenderer, mActiveTexture);
    ASSERT(success == 0,
           "Could not change render target " + std::string(SDL_GetError()));
  }

  Vector2DInt cameraTilePos = Camera::renderPosToTilePos(cameraPos);
  int endRenderX = cameraTilePos.x + numberOfTilesToRenderX;
  if (endRenderX >= mapSize.x) endRenderX = mapSize.x - 1;

  int endRenderY = cameraTilePos.y + numberOfTilesToRenderY;
  if (endRenderY >= mapSize.y) endRenderY = mapSize.y - 1;
  for (int x = 0; x < numberOfTilesToRenderX; ++x) {
    for (int y = 0; y < numberOfTilesToRenderY; ++y) {
      Vector2DInt pos{x + cameraTilePos.x, y + cameraTilePos.y};
      if (!mActiveGridMap.isPosInMap(pos)) continue;
      const Sprite& sprite = mActiveGridMap.getPosSprite(pos);
      SDL_Texture* text = sprite.getSdlTexture();
      int renderPosX = GridMap::tileRenderSize.x * x;
      int renderPosY = GridMap::tileRenderSize.y * y;
      SDL_Rect dstRect{renderPosX, renderPosY, GridMap::tileRenderSize.x,
                       GridMap::tileRenderSize.y};
      int success = SDL_RenderCopy(renderer, text,
                                   &sprite.getRect().getSdlRect(), &dstRect);
      ASSERT(success == 0, "Could not render " + std::string(SDL_GetError()));
    }
  }
  {
    int success = SDL_SetRenderTarget(GraphicsManager::mMainRenderer, NULL);
    ASSERT(success == 0,
           "Could not restore render target " + std::string(SDL_GetError()));
  }
}

GridMapRenderer::GridMapRenderer(GameObject& g)
    : Component(g), mActiveGridMap(GridMap::getActiveMap()) {}

void GridMapRenderer::update() {
  Vector2DInt cameraPos{Camera::get().getPosition()};
  if (mPreviousCameraPosition != cameraPos) {
    prepareViewedArea();
    mPreviousCameraPosition = cameraPos;
  }
}
void GridMapRenderer::render() {
  if (mActiveTexture) {
    GraphicsManager::renderTexture(mActiveTexture, {0, 0});
  }
}