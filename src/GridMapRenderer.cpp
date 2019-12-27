#include "GridMapRenderer.h"

#include "GraphicsManager.h"
#include "Helpers.h"
#include "Paths.h"
#include "SpriteLoader.h"
#include "Tile.h"
#include "Vector2DInt.h"
void GridMapRenderer::renderTile(const Tile& tile, const Vector2DInt& pos) {
  switch (tile.mType) {
    case Tile::grass:
      GraphicsManager::renderTexture(*grassSprite, pos);
      break;
    case Tile::dirt:
      GraphicsManager::renderTexture(*dirtSprite, pos);
      break;
    default:
      break;
  }
}

GridMapRenderer::GridMapRenderer(GameObject& g)
    : Component(g),
      mActiveGridMap(GridMap::getActiveMap()),
      grassSprite(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                                  Paths::SIZE_OF_GRASS_TILE)),
      dirtSprite(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {0, 2},
                                                 Paths::SIZE_OF_GRASS_TILE)) {}

void GridMapRenderer::render() {
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

  for (int x = 0; x < numberOfTilesToRenderX; ++x) {
    for (int y = 0; y < numberOfTilesToRenderY; ++y) {
      const Tile* tile = mActiveGridMap.getTile({x, y});
      ASSERT(tile, "got null tileptr");
      renderTile(*tile, {x * tileSize.x, y * tileSize.y});
    }
  }

  /*
      for each tile from cameraPos to cameraPos + as many tiles as fit in a
     screen The gridmap has a size each tile has a size The camera has a scaling
  */
}