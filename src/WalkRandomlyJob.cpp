#include "WalkRandomlyJob.h"

#include <memory>

#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
namespace {
class WalkingState;
class WaitingState;
class CalculatingPositionState;

class WalkingState : public State {
 public:
  WalkingState(GameObject& user, const Vector3DInt& targetPos);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  DorfWalker mWalker;
  GameObject& mUser;
  Vector3DInt mTargetPos;
};

class WaitingState : public State {
 public:
  WaitingState(int durationMs);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  Timer mTimer;
  int mWaitingTimeMs{3000};
};

class CalculatingPositionState : public State {
 public:
  CalculatingPositionState(GameObject& user);
  std::unique_ptr<State> onDuring() override;

 private:
  GameObject& mUser;
};

WalkingState::WalkingState(GameObject& user, const Vector3DInt& targetPos)
    : mUser(user), mTargetPos(targetPos) {}
void WalkingState::onEntry() {
  mWalker.generateNewPath(mUser.getPosition(), mTargetPos);
}
std::unique_ptr<State> WalkingState::onDuring() {
  if (mWalker.isDone()) {
    return std::make_unique<WaitingState>(3000);
  } else {
    Vector3DInt pos = mUser.getPosition();
    mWalker.walkUpdate(pos);
    mUser.setPosition(pos);
    return nullptr;
  }
}

WaitingState::WaitingState(int durationMs) : mWaitingTimeMs(durationMs) {}
void WaitingState::onEntry() { mTimer.start(); }

std::unique_ptr<State> WaitingState::onDuring() {
  if (mTimer.getElapsedMilliseconds() > mWaitingTimeMs) {
    terminateMachine();
    return nullptr;
  }
  return nullptr;
}

CalculatingPositionState::CalculatingPositionState(GameObject& user)
    : mUser(user) {}

std::unique_ptr<State> CalculatingPositionState::onDuring() {
  int x = Helpers::randomInt(-10, 10);
  int y = Helpers::randomInt(-10, 10);
  int z = Helpers::randomInt(-10, 10);
  Vector3DInt newPos = mUser.getPosition() + Vector3DInt{x, y, z};
  const GridMap& map = GridMap::getActiveMap();
  if (map.isPosInMap(newPos) && map.isPosFree(newPos)) {
    bool success = map.getLowestPassablePositionFrom(newPos, newPos);
    if (success) {
      return std::make_unique<WalkingState>(mUser, newPos);
    }
  }
  return std::make_unique<WaitingState>(200);
}

}  // namespace

WalkRandomlyJob::WalkRandomlyJob(GameObject& user)
    : mDorf(user),
      mStateMachine(
          std::move(std::make_unique<CalculatingPositionState>(user))) {}

bool WalkRandomlyJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}