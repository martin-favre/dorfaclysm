#include "Astar.h"

#include <gtest/gtest.h>

#include "GridMap.h"
#include "Timer.h"
#include "Vector3DInt.h"

void generateEmptyWorld(GridMap& gridMap, const Vector3DInt& size) {
  (void)gridMap;
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        const Vector3DInt pos{x, y, z};
        // gridMap.setBlockAt(pos, std::make_unique<AirBlock>());
      }
    }
  }
}

TEST(Astar, getPath_NoObstacles_ShouldFindPath) {
  GridMap::generateActiveMap({20, 20, 20}, generateEmptyWorld);
  const GridMap& map = GridMap::getActiveMap();

  Vector3DInt from(0, 0, 0);
  Vector3DInt to(0, 2, 0);
  std::stack<Vector3DInt> path;
  Astar().getPath(from, to, map, path);
  Vector3DInt nextStep = path.top();
  ASSERT_TRUE(nextStep == Vector3DInt(0, 1, 0));
  path.pop();
  ASSERT_EQ(path.top(), Vector3DInt(0, 2, 0));
  path.pop();
  ASSERT_TRUE(path.empty());
}

TEST(Astar, getPath_Profile) {
  int totalTime = 0;
  GridMap::generateActiveMap({500, 500, 1}, generateEmptyWorld);
  const GridMap& map = GridMap::getActiveMap();
  Timer t;
  for (int i = 0; i < 100; ++i) {
    constexpr Vector3DInt from(0, 0, 0);
    constexpr Vector3DInt to(499, 499, 0);
    std::stack<Vector3DInt> path;
    t.start();
    bool success = Astar().getPath(from, to, map, path);
    ASSERT_TRUE(success);
    totalTime += t.getElapsedMilliseconds();
  }
  std::cout << "Took: " << totalTime << "ms" << std::endl;
}