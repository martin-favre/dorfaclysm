
#include "WalkRandomlyJob.h"

#include <memory>

#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
namespace {

class WalkingState : public State {
 public:
  WalkingState(GridActor& user, const Vector3DInt& targetPos)
      : mWalker(user), mTargetPos(targetPos) {}
  void onEntry() override { mWalker.startNewPath(mTargetPos); }
  std::unique_ptr<State> onDuring() override;

 private:
  DorfWalker mWalker;
  Vector3DInt mTargetPos;
};

class WaitingState : public State {
 public:
  WaitingState(int durationMs) : mWaitingTimeMs(durationMs) {}
  void onEntry() override { mTimer.start(); }
  std::unique_ptr<State> onDuring() override;

 private:
  Timer mTimer;
  int mWaitingTimeMs{3000};
};

class CalculatingPositionState : public State {
 public:
  CalculatingPositionState(GridActor& user) : mUser(user) {}
  std::unique_ptr<State> onDuring() override;

 private:
  GridActor& mUser;
};

std::unique_ptr<State> WalkingState::onDuring() {
  mWalker.update();
  if (mWalker.isIdle()) {
    return transitTo<WaitingState>(3000);
  }
  return noTransition();
}

std::unique_ptr<State> WaitingState::onDuring() {
  if (mTimer.getElapsedMilliseconds() > mWaitingTimeMs) {
    terminateMachine();
    return noTransition();
  }
  return noTransition();
}

std::unique_ptr<State> CalculatingPositionState::onDuring() {
  int x = Helpers::randomInt(-10, 10);
  int y = Helpers::randomInt(-10, 10);
  int z = Helpers::randomInt(-10, 10);
  Vector3DInt newPos = mUser.getCurrentPos() + Vector3DInt{x, y, z};
  const GridMap& map = GridMap::getActiveMap();
  if (map.isPosInMap(newPos) && map.isPosFree(newPos)) {
    bool success = map.getLowestPassablePositionFrom(newPos, newPos);
    if (success) {
      return transitTo<WalkingState>(mUser, newPos);
    }
  }
  return transitTo<WaitingState>(200);
}

}  // namespace

WalkRandomlyJob::WalkRandomlyJob(GridActor& user)
    : mStateMachine(std::make_unique<CalculatingPositionState>(user)) {}

bool WalkRandomlyJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}