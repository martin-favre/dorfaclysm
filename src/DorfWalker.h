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
  DorfWalker(GridActor& gridActor, const SerializedObj& serObj);
  Vector3DInt getNextPlannedPosition();
  void update();
  void startNewPath(const Vector3DInt& to);
  bool isCalculating() const;
  bool isWalking() const;
  bool isIdle() const;
  bool hasFailed() const;
  FailureReason getFailReason() const;
  friend void to_json(SerializedObj& serObj, const DorfWalker& walker); 
 private:
  void calculateNewPath();
  void walk();

  GridActor& mGridActor;
  const GridMap& mGridMap;
  std::unique_ptr<IncrementalAstar> mAstar;

  WalkerState mState{idle};
  FailureReason mFail{noFailure};
  int mMsPerMovement;
  Vector3DInt mGoal;
  Timer mTimer;
  std::stack<Vector3DInt> mPlannedPosititions;
};

void to_json(SerializedObj& serObj, const DorfWalker& walker);