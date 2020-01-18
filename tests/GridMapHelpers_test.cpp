#include "GridMapHelpers.h"

#include <gtest/gtest.h>

#include "DefaultBlock.h"
#include "GridMap.h"
#include "Vector2DInt.h"

class UnPassableBlock : public DefaultBlock {
 public:
  bool mayPassThrough() const override { return false; }
};

class PassableBlock : public DefaultBlock {
 public:
  bool mayPassThrough() const override { return true; }
};

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
        map.setBlockAt({0, 0}, std::make_unique<UnPassableBlock>());
        map.setBlockAt({1, 0}, std::make_unique<UnPassableBlock>());
        map.setBlockAt({2, 0}, std::make_unique<UnPassableBlock>());

        map.setBlockAt({0, 1}, std::make_unique<UnPassableBlock>());
        map.setBlockAt({1, 1}, std::make_unique<UnPassableBlock>());
        map.setBlockAt({2, 1}, std::make_unique<UnPassableBlock>());

        map.setBlockAt({0, 2}, std::make_unique<PassableBlock>());
        map.setBlockAt({1, 2}, std::make_unique<PassableBlock>());
        map.setBlockAt({2, 2}, std::make_unique<PassableBlock>());

        map.setBlockAt({0, 3}, std::make_unique<UnPassableBlock>());
        map.setBlockAt({1, 3}, std::make_unique<UnPassableBlock>());
        map.setBlockAt({2, 3}, std::make_unique<UnPassableBlock>());
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
