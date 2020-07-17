#include "BlockType.h"
#include "IncrementalAstar.h"

#include <gtest/gtest.h>

#include "GridMap.h"
#include "Timer.h"
#include "Vector3DInt.h"

void generateEmptyWorld(GridMap& gridMap, const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        const Vector3DInt pos{x, y, z};
        gridMap.setBlockAt(pos, BlockTypeAirBlock);
      }
    }
  }
}

TEST(IncrementalAstar_test, getPath_NoObstacles_ShouldFindPath) {
  GridMap::generateActiveMap({20, 20, 20}, generateEmptyWorld);
  const GridMap& map = GridMap::getActiveMap();

  Vector3DInt from(0, 0, 0);
  Vector3DInt to(0, 2, 0);
  IncrementalAstar astar{from, to, map, 0};
  while (!astar.isDone()) {
    astar.calculatePath();
  }
  ASSERT_TRUE(astar.foundPath());

  std::stack<Vector3DInt> path;
  astar.getPath(path);
  Vector3DInt nextStep = path.top();
  ASSERT_TRUE(nextStep == Vector3DInt(0, 1, 0));
  path.pop();
  ASSERT_EQ(path.top(), Vector3DInt(0, 2, 0));
  path.pop();
  ASSERT_TRUE(path.empty());
}

TEST(IncrementalAstar_test, getPath_Profile) {
  int totalTime = 0;
  GridMap::generateActiveMap({500, 500, 1}, generateEmptyWorld);
  const GridMap& map = GridMap::getActiveMap();
  Timer t;
  for (int i = 0; i < 100; ++i) {
    constexpr Vector3DInt from(0, 0, 0);
    constexpr Vector3DInt to(499, 499, 0);
    t.start();
    IncrementalAstar astar{from, to, map, 0};
    while (!astar.isDone()) {
      astar.calculatePath();
    }
    ASSERT_TRUE(astar.foundPath());
    std::stack<Vector3DInt> path;
    astar.getPath(path);
    while (!path.empty()) {
      std::cout << path.top() << std::endl;
      path.pop();
    }
    totalTime += t.getElapsedMilliseconds();
  }
  std::cout << "Took: " << totalTime << "ms" << std::endl;
}