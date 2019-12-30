#include "TileFloor.h"
namespace {
std::unique_ptr<Sprite> grassSprite;

std::unique_ptr<Sprite> dirtSprite;

std::unique_ptr<Sprite> rockSprite;

std::unique_ptr<Sprite> openSprite;

}  // namespace

TileFloor getGrassTileFloor() {
  if (!grassSprite) {
    grassSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                                  Paths::SIZE_OF_GRASS_TILE);
  }
  return TileFloor(*grassSprite);
}
TileFloor getDirtTileFloor() {
  if (!dirtSprite) {
    dirtSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {0, 2},
                                                 Paths::SIZE_OF_GRASS_TILE);
  }
  return TileFloor(*dirtSprite);
}
TileFloor getRockTileFloor() {
  if (!rockSprite) {
    rockSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 6},
                                                 Paths::SIZE_OF_GRASS_TILE);
  }
  return TileFloor(*rockSprite);
}

TileFloor getOpenTileFloor() {
  if (!openSprite) {
    openSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {5, 5},
                                                 Paths::SIZE_OF_GRASS_TILE);
  }
  return TileFloor(*openSprite);
}
