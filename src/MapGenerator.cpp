#include "MapGenerator.h"

#include "Tile.h"
#include "Vector2DInt.h"
void MapGenerator::generate(GridMap& gridMap, const Vector2DInt& size) {
  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      Vector2DInt pos{x, y};
      Tile newTile;
      if ((x + y) % 2 == 0) {
        newTile.mType = Tile::grass;
      } else {
        newTile.mType = Tile::dirt;
      }

      gridMap.setTile(pos, newTile);
    }
  }
}