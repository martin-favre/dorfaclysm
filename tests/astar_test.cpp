#include "Astar.h"

#include <gtest/gtest.h>

#include "GridMap.h"
#include "Timer.h"
#include "Vector2DInt.h"
TEST(Astar, getPath_NoObstacles_ShouldFindPath) {
  /**
   * ###
   * ###
   * ###
   */
  GridMap map(Vector2DInt(3, 3));
  Vector2DInt from(0, 0);
  Vector2DInt to(2, 2);
  std::stack<Vector2DInt> path;
  Astar().getPath(from, to, map, path);
  ASSERT_EQ(path.top(), Vector2DInt(1, 0));
  path.pop();
  ASSERT_EQ(path.top(), Vector2DInt(1, 1));
  path.pop();
  ASSERT_EQ(path.top(), Vector2DInt(1, 2));
  path.pop();
  ASSERT_EQ(path.top(), Vector2DInt(2, 2));
  path.pop();
  ASSERT_TRUE(path.empty());
}

TEST(Astar, getPath_Profile) {
  GridMap map(Vector2DInt(1000, 1000));
  Vector2DInt from(0, 0);
  Vector2DInt to(999, 999);
  std::stack<Vector2DInt> path;
  Timer t;
  t.start();
  Astar().getPath(from, to, map, path);
  int time = t.getElapsedMilliseconds();
  std::cout << "Took: " << time << "ms" << std::endl;
}