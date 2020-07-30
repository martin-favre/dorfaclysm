#include "BlockType.h"
#include "GridMapHelpers.h"

#include <gtest/gtest.h>

#include "GridMap.h"
#include "Vector2DInt.h"

bool rowIsExplored(int y, const GridMap& map) {
  for (int x = 0; x < map.getSize().x; ++x) {
    const Block& block = map.getBlockAt({x, y});
    if (!block.isExplored()) return false;
  }
  return true;
}

bool rowIsUnExplored(int y, const GridMap& map) {
  for (int x = 0; x < map.getSize().x; ++x) {
    const Block& block = map.getBlockAt({x, y});
    if (block.isExplored()) return false;
  }
  return true;
}

TEST(TestGridMap, getLowestPassablePositionFrom_TopOfWorld) {
  std::function<void(GridMap&, const Vector3DInt&)> foo =
      [](GridMap& map, const Vector3DInt& size) {
        (void)size;
        /*
        ###
        ###
        ...
        ###
        */
        map.setBlockAt({0, 0}, Block(BlockTypeRockBlock));
        map.setBlockAt({1, 0}, Block(BlockTypeRockBlock));
        map.setBlockAt({2, 0}, Block(BlockTypeRockBlock));

        map.setBlockAt({0, 1}, Block(BlockTypeRockBlock));
        map.setBlockAt({1, 1}, Block(BlockTypeRockBlock));
        map.setBlockAt({2, 1}, Block(BlockTypeRockBlock));

        map.setBlockAt({0, 2}, Block(BlockTypeAirBlock));
        map.setBlockAt({1, 2}, Block(BlockTypeAirBlock));
        map.setBlockAt({2, 2}, Block(BlockTypeAirBlock));

        map.setBlockAt({0, 3}, Block(BlockTypeRockBlock));
        map.setBlockAt({1, 3}, Block(BlockTypeRockBlock));
        map.setBlockAt({2, 3}, Block(BlockTypeRockBlock));
      };
  GridMap::generateActiveMap({3, 4, 1}, foo);
  GridMap& map = GridMap::getActiveMap();
  GridMapHelpers::exploreMap(map, {0, 2});
  ASSERT_TRUE(rowIsUnExplored(0, map));
  ASSERT_TRUE(rowIsExplored(1, map));
  ASSERT_TRUE(rowIsExplored(2, map));
  ASSERT_TRUE(rowIsExplored(3, map));
  GridMapHelpers::exploreMap(map, {0, 2});
  ASSERT_TRUE(rowIsUnExplored(0, map));
  ASSERT_TRUE(rowIsExplored(1, map));
  ASSERT_TRUE(rowIsExplored(2, map));
  ASSERT_TRUE(rowIsExplored(3, map));
}


TEST(TestGridMap, getClosestFreePositionToTest)
{
  std::function<void(GridMap&, const Vector3DInt&)> foo =
      [](GridMap& map, const Vector3DInt& size) {
        /*
        ..###
        ..###
        ..###
        .....
        */
        for(int x = 0; x < size.x; ++x)
        {
          for(int y = 0; y < size.y; ++y)
          {
            if(x > 1 && y < 3)
            {
              map.setBlockAt({x, y}, Block(BlockTypeRockBlock));
            }
            else
            {
              map.setBlockAt({x, y}, Block(BlockTypeAirBlock));
            }
          }
        }
      };
  GridMap::generateActiveMap({5, 5, 1}, foo);
  GridMap& map = GridMap::getActiveMap();
  {
    Vector3DInt out;
    bool success = GridMapHelpers::getClosestFreePositionTo(map, {0,0}, out);
    ASSERT_TRUE(success);
    ASSERT_EQ(out, Vector3DInt(0,0));
  }
  {
    Vector3DInt out;
    bool success = GridMapHelpers::getClosestFreePositionTo(map, {2,0}, out);
    ASSERT_TRUE(success);
    ASSERT_EQ(out, Vector3DInt(1,0));
  }
  {
    Vector3DInt out;
    bool success = GridMapHelpers::getClosestFreePositionTo(map, {3,0}, out);
    ASSERT_FALSE(success);
  }
}