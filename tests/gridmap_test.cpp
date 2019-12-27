#include "GridMap.h"

#include <gtest/gtest.h>

#include "Vector2DInt.h"
TEST(Astar, defaultGeneration_ShouldYieldValidMap) {
  Vector2DInt mapSize{1000, 500};
  GridMap::generateActiveMap(mapSize, nullptr);
  GridMap& map = GridMap::getActiveMap();
  for (int y = 0; y < mapSize.y; ++y) {
    for (int x = 0; x < mapSize.x; ++x) {
      const Tile* t = map.getTile({x, y});
      ASSERT_TRUE(t != nullptr);
    }
  }

  ASSERT_TRUE(map.getTile({-1, 0}) == nullptr);
  ASSERT_TRUE(map.getTile({0, -1}) == nullptr);
  ASSERT_TRUE(map.getTile({-1, -1}) == nullptr);
  ASSERT_TRUE(map.getTile({mapSize.x, 0}) == nullptr);
  ASSERT_TRUE(map.getTile({0, mapSize.y}) == nullptr);
}

TEST(Astar, defaultGeneration_RegenerationShouldClearMap) {
  Vector2DInt mapSize{2, 2};
  GridMap& map = GridMap::generateActiveMap(mapSize, nullptr);
  ASSERT_TRUE(map.getTile({0, 0})->mOpen);
  Tile closedTile;
  closedTile.mOpen = false;
  map.setTile({0, 0}, closedTile);
  ASSERT_FALSE(map.getTile({0, 0})->mOpen);
  GridMap::generateActiveMap(mapSize, nullptr);
  ASSERT_TRUE(map.getTile({0, 0})->mOpen);
}