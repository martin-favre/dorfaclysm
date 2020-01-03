#include "MapGenerator.h"

#include "Tile.h"
#include "TileFloor.h"
#include "Vector2DInt.h"
#include "WorldTile.h"
void MapGenerator::generateFloor(GridMap& gridMap, const Vector2DInt& size) {
  for (int y = 0; y < size.y; ++y) {
    gridMap.mTiles.emplace_back();
    for (int x = 0; x < size.x; ++x) {
      if ((x + y) % 2 == 0) {
        gridMap.mTiles[y].emplace_back(std::make_unique<GrassTileFloor>());
      } else {
        gridMap.mTiles[y].emplace_back(std::make_unique<DirtTileFloor>());
      }
    }
  }
}

void MapGenerator::generateRocks(GridMap& gridMap, const Vector2DInt& size) {
  (void)gridMap;
  (void)size;
  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      if (y < size.y / 2 && x > size.x / 2) {
        gridMap.mTiles[y][x].mTile = std::make_unique<RockTile>();
        gridMap.mTiles[y][x].mFloor = std::make_unique<RockTileFloor>();
      }
      else if(x > 5 && x < 10 && y < 10)
      {
        gridMap.mTiles[y][x].mTile = std::make_unique<RockTile>();
        gridMap.mTiles[y][x].mFloor = std::make_unique<RockTileFloor>();
      }
    }
  }
}

void MapGenerator::generate(GridMap& gridMap, const Vector2DInt& size) {
  generateFloor(gridMap, size);
  generateRocks(gridMap, size);
}