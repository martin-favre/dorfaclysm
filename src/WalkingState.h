#pragma once
#include "DorfWalker.h"
#include "GridMapHelpers.h"
#include "StateMachine.h"
class WalkingState : public State {
 public:
  WalkingState(GridActor& user, int msPerStep) : mWalker(user, msPerStep) {}

  virtual Vector3DInt getTargetPos() = 0;
  virtual std::unique_ptr<State> onReachedTarget() = 0;
  virtual void onPathFindFail() = 0;

  void onEntry() override {
    GridMap& map = GridMap::getActiveMap();
    Vector3DInt movePos;
    bool success =
        GridMapHelpers::getClosestFreePositionTo(map, getTargetPos(), movePos);
    if (success) {
      mWalker.startNewPath(movePos);
    } else {
      onPathFindFail();
    }
  }

  std::unique_ptr<State> onDuring() override {
    mWalker.update();
    if (mWalker.isIdle()) {
      if (!mWalker.hasFailed()) {
        return onReachedTarget();
      } else {
        onPathFindFail();
        terminateMachine();
        return noTransition();
      }
    }
    return noTransition();
  }

 private:
  DorfWalker mWalker;
};