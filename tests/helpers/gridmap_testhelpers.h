
#include "BlockType.h"
#include "GridMap.h"

inline void generateFlatWorld(GridMap& gridMap, const Vector3DInt& size) {
  int firstPassableHeight = size.z / 2;
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        const Vector3DInt pos{x, y, z};
        if (z < firstPassableHeight) {
          gridMap.setBlockAt(pos, Block(BlockTypeRockBlock));
        } else {
          gridMap.setBlockAt(pos, Block(BlockTypeAirBlock));
        }
      }
    }
  }
}
