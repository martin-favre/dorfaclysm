#pragma once

#include <stack>

#include "Timer.h"
#include "Vector2DInt.h"

class DorfWalker {
 public:
  Vector2DInt getNextPlannedPosition();
  void walkUpdate(Vector2DInt& currentPosition);
  bool isDone();
  bool generateNewPath(const Vector2DInt& from, const Vector2DInt& to);

 private:
  std::stack<Vector2DInt> mPlannedPosititions;
  int mMsPerMovement{500};
  Timer mTimer;
};