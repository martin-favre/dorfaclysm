#include "MapGenerator.h"

#include "AirBlock.h"
#include "BlockType.h"
#include "Engine.h"
#include "GrassBlock.h"
#include "Helpers.h"
#include "RockBlock.h"
#include "StairUpDownBlock.h"
#include "Tree.h"
void MapGenerator::generateFlatWorld(GridMap& gridMap,
                                     const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        const Vector3DInt pos{x, y, z};
        if (z < size.z / 2 - 1) {
          gridMap.setBlockAt(pos, BlockTypeRockBlock);
        } else if (z == (size.z / 2) - 1) {
          gridMap.setBlockAt(pos, BlockTypeGrassBlock);
        } else {
          gridMap.setBlockAt(pos, BlockTypeAirBlock);
        }
      }
    }
  }
}

void MapGenerator::generateStairWorld(GridMap& gridMap,
                                      const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        const Vector3DInt pos{x, y, z};
        if (z == x && y == 5) {
          gridMap.setBlockAt(pos, BlockTypeStairUpDownBlock);
        } else if (z == x) {
          gridMap.setBlockAt(pos, BlockTypeGrassBlock);
        } else if (z < x) {
          gridMap.setBlockAt(pos, BlockTypeRockBlock);
        } else {
          gridMap.setBlockAt(pos, BlockTypeAirBlock);
          if (z > 0 && !gridMap.getBlockAt({x, y, z - 1}).mayPassThrough()) {
            if (Helpers::randomInt(0, 100) > 90) {
              // Engine::addGameObject<Tree>().setPosition({x, y, z});
            }
          }
        }
      }
    }
  }
}

void MapGenerator::generate(GridMap& gridMap, const Vector3DInt& size) {
  generateStairWorld(gridMap, size);
  // generateFlatWorld(gridMap, size);
}
