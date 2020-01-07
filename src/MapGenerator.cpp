#include "MapGenerator.h"

#include "GrassBlock.h"
#include "AirBlock.h"
#include "RockBlock.h"

void MapGenerator::generate(GridMap& gridMap, const Vector3DInt& size) {

  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        if(z < size.z/2-1)
        {
          gridMap.mBlocks[z][y][x] = std::make_unique<RockBlock>();
        }
        else if(z == size.z/2-1)
        {
          gridMap.mBlocks[z][y][x] = std::make_unique<GrassBlock>();
        }
        else
        {
          gridMap.mBlocks[z][y][x] = std::make_unique<AirBlock>();
        }
      }
    }
  }
}