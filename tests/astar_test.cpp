#include "Astar.h"

#include <gtest/gtest.h>

#include "GridMap.h"
#include "Timer.h"
#include "Vector2DInt.h"
TEST(Astar, getPath_NoObstacles_ShouldFindPath) {
  /*
  GridMap::generateActiveMap(Vector2DInt(1000, 1000), nullptr);
  const GridMap& map = GridMap::getActiveMap();

  Vector2DInt from(0, 0);
  Vector2DInt to(2, 2);
  std::stack<Vector2DInt> path;
  Astar().getPath(from, to, map, path);
  Vector2DInt nextStep = path.top();
  ASSERT_TRUE(nextStep == Vector2DInt(1, 0) || nextStep == Vector2DInt(0, 1));
  path.pop();
  ASSERT_EQ(path.top(), Vector2DInt(1, 1));
  path.pop();
  nextStep = path.top();
  ASSERT_TRUE(nextStep == Vector2DInt(1, 2) || nextStep == Vector2DInt(2, 1));
  path.pop();
  ASSERT_EQ(path.top(), Vector2DInt(2, 2));
  path.pop();
  ASSERT_TRUE(path.empty());
  */
}

TEST(Astar, getPath_Profile) {

/*
  GridMap::generateActiveMap(Vector3DInt(256, 256, 256), nullptr);
  const GridMap& map = GridMap::getActiveMap();
  Vector3DInt from(0, 0, 0);
  Vector3DInt to(255, 255, 0);
  std::stack<Vector3DInt> path;
  Timer t;
  t.start();
  bool success = Astar().getPath(from, to, map, path);
  ASSERT_TRUE(success);
  int time = t.getElapsedMilliseconds();
  std::cout << "Took: " << time << "ms" << std::endl;
*/
}