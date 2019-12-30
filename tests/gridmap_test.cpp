#include "GridMap.h"

#include <gtest/gtest.h>

#include "Vector2DInt.h"
TEST(Astar, defaultGeneration_ShouldYieldValidMap) {
  /*
  Vector2DInt mapSize{1000, 500};
  GridMap::generateActiveMap(mapSize, nullptr);
  GridMap& map = GridMap::getActiveMap();
  for (int y = 0; y < mapSize.y; ++y) {
    for (int x = 0; x < mapSize.x; ++x) {
      ASSERT_TRUE(map.isPosInMap({x,y}));
      const Tile& t = map.getTile({x, y});
      (void)t;
    }
  }

  ASSERT_FALSE(map.isPosInMap({-1, 0}));
  ASSERT_FALSE(map.isPosInMap({0, -1}));
  ASSERT_FALSE(map.isPosInMap({-1, -1}));
  ASSERT_FALSE(map.isPosInMap({mapSize.x, 0}));
  ASSERT_FALSE(map.isPosInMap({0, mapSize.y}));
  */
}

TEST(Astar, defaultGeneration_RegenerationShouldClearMap) {
  /*
  Vector2DInt mapSize{2, 2};
  GridMap& map = GridMap::generateActiveMap(mapSize, nullptr);
  ASSERT_TRUE(map.getTile({0, 0}).mOpen);
  Tile closedTile;
  closedTile.mOpen = false;
  map.setTile({0, 0}, closedTile);
  ASSERT_FALSE(map.getTile({0, 0}).mOpen);
  GridMap::generateActiveMap(mapSize, nullptr);
  ASSERT_TRUE(map.getTile({0, 0}).mOpen);
  */
}