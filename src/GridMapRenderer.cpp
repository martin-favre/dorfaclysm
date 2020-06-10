#include "GridMapRenderer.h"

#include "Block.h"
#include "Camera.h"
#include "Component.h"
#include "GraphicsManager.h"
#include "Helpers.h"
#include "Logging.h"
#include "Paths.h"
#include "Serializer.h"
#include "SpriteLoader.h"
#include "Vector2DInt.h"

void GridMapRenderer::setup() { prepareViewedArea(); }

/**
 * Rendering the whole map, sprite for sprite, is performance expensive.
 * Instead pre-generate a texture.
 * Generating a texture for the whole map is memory expensive.
 * So generate just the current screen.
 */

const Sprite* getSeeThroughSprite(const GridMap& gridmap, Vector3DInt pos) {
  /*

  */
  int depth = 0;
  constexpr uint8_t maxDepth = 20;
  while (gridmap.getBlockAt(pos).isSeeThrough()) {
    pos += {0, 0, -1};
    ++depth;
    if (depth >= maxDepth) break;
  }
  if (depth < maxDepth) {
    return gridmap.getBlockAt(pos).getTopSprite();
  }
  return nullptr;
}

void GridMapRenderer::prepareViewedArea() {
  Vector3DInt cameraPos = Camera::get().getPosition();
  Vector2DInt tileSize{Camera::tileRenderSize};
  const Vector3DInt mapSize{mActiveGridMap.getSize()};
  Vector2DInt screenSize{GraphicsManager::getScreenWidth(),
                         GraphicsManager::getScreenHeight()};

  const int numberOfTilesToRenderX = screenSize.x / tileSize.x + 1;
  const int numberOfTilesToRenderY = screenSize.y / tileSize.y + 1;

  // SDL_Renderer* renderer = GraphicsManager::mMainRenderer;

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
      // if (!block.isExplored()) continue;
      const int renderPosX = Camera::tileRenderSize.x * x;
      const int renderPosY = Camera::tileRenderSize.y * y;

      const Sprite* sprite{nullptr};
      if (block.isSeeThrough()) {
        sprite = getSeeThroughSprite(mActiveGridMap, pos);
      } else {
        // sprite = block.getTopSprite();
        GraphicsManager::setRenderDrawColor({158, 87, 0, SDL_ALPHA_OPAQUE});
        GraphicsManager::drawRect({renderPosX, renderPosY,
                                   Camera::tileRenderSize.x,
                                   Camera::tileRenderSize.y});
        GraphicsManager::setRenderDrawColor(GraphicsManager::mDefaultDrawColor);
      }
      if (sprite) {
        GraphicsManager::renderTexture(*sprite, {renderPosX, renderPosY});
      }
    }
  }
}

GridMapRenderer::GridMapRenderer(GameObject& g)
    : Component(g), mActiveGridMap(GridMap::getActiveMap()) {}

GridMapRenderer::GridMapRenderer(GameObject& g, const SerializedObj& serObj)
    : Component(g, serObj["parent"]), mActiveGridMap(GridMap::getActiveMap()) {}

SerializedObj GridMapRenderer::serialize() const {
  SerializedObj out = createSerializedObj<GridMapRenderer>();
  out["parent"] = Component::serialize();
  return out;
}

void GridMapRenderer::update() {}
void GridMapRenderer::render() { prepareViewedArea(); }

std::string GridMapRenderer::getTypeString() { return "GridMapRenderer"; }