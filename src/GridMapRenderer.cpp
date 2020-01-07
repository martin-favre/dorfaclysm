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

void renderSeeThroughSprite(const GridMap& gridmap, Vector3DInt pos,
                                     SDL_Renderer* renderer,
                                     const SDL_Rect& dstRect) {
  /*

  */
  int depth = 1;
  constexpr uint8_t maxDepth = 10;
  while (gridmap.getBlockAt(pos).isSeeThrough()) {
    pos += {0, 0, -1};
    ++depth;
    if(depth >= maxDepth) break;
  }
  if (depth < maxDepth) {
    const Sprite* sprite = gridmap.getBlockAt(pos).getSprite();
    int success = SDL_RenderCopy(renderer, sprite->getSdlTexture(),
                                 &sprite->getRect().getSdlRect(), &dstRect);
    ASSERT(success == 0,
            "Could not render " + std::string(SDL_GetError()));
  }

  uint8_t alpha = depth*255/maxDepth;
  
  GraphicsManager::setRenderDrawColor({0,0,255,alpha});
  GraphicsManager::drawRect({dstRect});
  GraphicsManager::setRenderDrawColor(GraphicsManager::mDefaultDrawColor);

}

void GridMapRenderer::prepareViewedArea() {
  std::unique_ptr<Sprite> tmpEmptySprite = SpriteLoader::loadSpriteByIndex(
      Paths::BROWN_SQUARE, {0, 0}, Paths::SIZE_OF_BROWN_SQUARE);
  Vector3DInt cameraPos = Camera::get().getPosition();
  Vector2DInt tileSize{GridMap::tileRenderSize};
  const Vector3DInt mapSize{mActiveGridMap.getSize()};
  Vector2DInt screenSize{GraphicsManager::getScreenWidth(),
                         GraphicsManager::getScreenHeight()};

  const int numberOfTilesToRenderX = screenSize.x / tileSize.x + 1;
  const int numberOfTilesToRenderY = screenSize.y / tileSize.y + 1;

  SDL_Renderer* renderer = GraphicsManager::mMainRenderer;

  Vector2DInt cameraTilePos = Camera::renderPosToTilePos(cameraPos);
  int endRenderX = cameraTilePos.x + numberOfTilesToRenderX;
  if (endRenderX >= mapSize.x) endRenderX = mapSize.x - 1;

  int endRenderY = cameraTilePos.y + numberOfTilesToRenderY;
  if (endRenderY >= mapSize.y) endRenderY = mapSize.y - 1;
  for (int x = 0; x < numberOfTilesToRenderX; ++x) {
    for (int y = 0; y < numberOfTilesToRenderY; ++y) {
      Vector3DInt pos{x + cameraTilePos.x, y + cameraTilePos.y, cameraPos.z};
      if (!mActiveGridMap.isPosInMap(pos)) continue;
      const Block& block = mActiveGridMap.getBlockAt(pos);
      const int renderPosX = GridMap::tileRenderSize.x * x;
      const int renderPosY = GridMap::tileRenderSize.y * y;
      const SDL_Rect dstRect{renderPosX, renderPosY, GridMap::tileRenderSize.x,
                             GridMap::tileRenderSize.y};

      if (block.isSeeThrough()) {
        renderSeeThroughSprite(mActiveGridMap, pos, renderer, dstRect);
      } else {
        if (block.getSprite()) {
          const Sprite* sprite = block.getSprite();
          SDL_Texture* text = sprite->getSdlTexture();
          int success = SDL_RenderCopy(
              renderer, text, &sprite->getRect().getSdlRect(), &dstRect);
          ASSERT(success == 0,
                 "Could not render " + std::string(SDL_GetError()));
        }
      }
    }
  }
}

GridMapRenderer::GridMapRenderer(GameObject& g)
    : Component(g), mActiveGridMap(GridMap::getActiveMap()) {}

void GridMapRenderer::update() { prepareViewedArea(); }
void GridMapRenderer::render() {}