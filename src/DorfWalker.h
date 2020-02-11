#pragma once

#include <stack>

#include "GridActor.h"
#include "IncrementalAstar.h"
#include "Timer.h"
#include "Vector3DInt.h"
class GridMap;
class DorfWalker {
 public:
  enum WalkerState { idle, calculatingPath, walking };

  enum FailureReason { noFailure, noPathFound, pathInterrupted };

  DorfWalker(GridActor& gridActor, int msPerMovement = 500);
  Vector3DInt getNextPlannedPosition();
  void update();
  void startNewPath(const Vector3DInt& to);
  // WalkerState getState() const;
  bool isCalculating() const;
  bool isWalking() const;
  bool isIdle() const;
  bool hasFailed() const;
  FailureReason getFailReason() const;

 private:
  void calculateNewPath();
  void walk();
  WalkerState mState{idle};
  FailureReason mFail{noFailure};
  std::stack<Vector3DInt> mPlannedPosititions;
  int mMsPerMovement;
  GridActor& mGridActor;
  const GridMap& mGridMap;
  std::unique_ptr<IncrementalAstar> mAstar;
  Vector3DInt mGoal;
  Timer mTimer;
};