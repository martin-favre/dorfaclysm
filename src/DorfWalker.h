#pragma once

#include <stack>

#include "Timer.h"
#include "Vector3DInt.h"
class GridMap;
class DorfWalker {
 public:
  DorfWalker(int msPerMovement = 500);
  Vector3DInt getNextPlannedPosition();
  void walkUpdate(Vector3DInt& currentPosition);
  bool isDone();
  bool generateNewPath(const Vector3DInt& from, const Vector3DInt& to);

 private:
  std::stack<Vector3DInt> mPlannedPosititions;
  int mMsPerMovement;
  const GridMap& mGridMap;
  Timer mTimer;
};