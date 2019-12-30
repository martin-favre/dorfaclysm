#include "MapGenerator.h"

#include "Tile.h"
#include "TileFloor.h"
#include "Vector2DInt.h"

void MapGenerator::generateFloor(GridMap& gridMap, const Vector2DInt& size) {
  for (int y = 0; y < size.y; ++y) {
    std::vector<TileFloor> mFloorRow;
    for (int x = 0; x < size.x; ++x) {
      Vector2DInt pos{x, y};
      if ((x + y) % 2 == 0) {
        mFloorRow.emplace_back(getGrassTileFloor());
      } else {
        mFloorRow.emplace_back(getDirtTileFloor());
      }
    }
    gridMap.mTileFloors.emplace_back(mFloorRow);
  }
}

void MapGenerator::generateRocks(GridMap& gridMap, const Vector2DInt& size) {
  (void)gridMap;
  (void)size;
  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      if (y < size.y / 2 && x > size.x / 2) {
        gridMap.mTiles[y][x] = std::make_unique<RockTile>();
      }
    }
  }
}

void MapGenerator::generate(GridMap& gridMap, const Vector2DInt& size) {
  generateFloor(gridMap, size);
  generateRocks(gridMap, size);
}