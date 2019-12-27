#include "GridMapRenderer.h"

#include "GraphicsManager.h"
#include "Helpers.h"
#include "Paths.h"
#include "SpriteLoader.h"
#include "Tile.h"
#include "Vector2DInt.h"

Sprite& GridMapRenderer::getSpriteFromTile(const Tile& tile)
{
  switch (tile.mType) {
    case Tile::grass:
      return *mGrassSprite;
      break;
    case Tile::dirt:
      return *mDirtSprite;
      break;
    default:
      return *mGrassSprite;
      break;
  }

}

void GridMapRenderer::setup()
{
  prepareViewedArea();
}


/**
 * Rendering the whole map, sprite for sprite, is performance expensive.
 * Instead pre-generate a texture.
 * Generating a texture for the whole map is memory expensive.
 * So generate just the current screen.
 */
void GridMapRenderer::prepareViewedArea()
{
  Vector2DInt cameraPos{0, 0};
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

  mActiveTexture = SDL_CreateTexture(renderer,
                               GraphicsManager::mMainSurface->format->format,
                               SDL_TEXTUREACCESS_TARGET,
                               screenSize.x,
                               screenSize.y);
  ASSERT(mActiveTexture, "could not create texture " + std::string(SDL_GetError()));
  {
    int success = SDL_SetRenderTarget(GraphicsManager::mMainRenderer,
                          mActiveTexture);
    ASSERT(success != 0, "Could not change render target " + std::string(SDL_GetError()));
  }
  for (int x = 0; x < numberOfTilesToRenderX; ++x) {
    for (int y = 0; y < numberOfTilesToRenderY; ++y) {
      const Tile* tile = mActiveGridMap.getTile({x, y});
      ASSERT(tile, "got null tileptr");
      Sprite& sprite = getSpriteFromTile(*tile);
      SDL_Texture * text = sprite.getSdlTexture();
      int renderPosX = GridMap::tileRenderSize.x * x;
      int renderPosY = GridMap::tileRenderSize.y * y;
      SDL_Rect dstRect{renderPosX, renderPosY, GridMap::tileRenderSize.x, GridMap::tileRenderSize.y};
      int success = SDL_RenderCopy(renderer, text, &sprite.getRect().getSdlRect(), &dstRect);
      ASSERT(success != 0, "Could not render " + std::string(SDL_GetError()));
    }
  }
  {
    int success = SDL_SetRenderTarget(GraphicsManager::mMainRenderer,
                          NULL);
    ASSERT(success != 0, "Could not restore render target " + std::string(SDL_GetError()));
  }

}

GridMapRenderer::GridMapRenderer(GameObject& g)
    : Component(g),
      mActiveGridMap(GridMap::getActiveMap()),
      mGrassSprite(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                                  Paths::SIZE_OF_GRASS_TILE)),
      mDirtSprite(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {0, 2},
                                                 Paths::SIZE_OF_GRASS_TILE)) {}

void GridMapRenderer::render() {
  if(mActiveTexture)
  {
    GraphicsManager::renderTexture(mActiveTexture, {0,0});
  }
}