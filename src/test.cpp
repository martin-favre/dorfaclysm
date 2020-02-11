#if 0
#include <memory>

#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
#include "WalkRandomlyJob.h"
namespace {
class WalkingState;
class WaitingState;
class CalculatingPositionState;

class WalkingState : public State {
 public:
  WalkingState(GridActor& user, const Vector3DInt& targetPos)
      : mWalker(user), mUser(user), mTargetPos(targetPos) {}
  void onEntry() override {
    mWalker.startNewPath(mUser.getCurrentPos(), mTargetPos);
  }
  std::unique_ptr<State> onDuring() override {
    // if (mWalker.isDone()) {
    return std::make_unique<WaitingState>(3000);
    // } else {
    //   Vector3DInt pos = mUser.getPosition();
    //   mWalker.walkUpdate(pos);
    //   mUser.setPosition(pos);
    //   return nullptr;
    // }
  }

 private:
  DorfWalker mWalker;
  GridActor& mUser;
  Vector3DInt mTargetPos;
};

class WaitingState : public State {
 public:
  WaitingState(int durationMs) : mWaitingTimeMs(durationMs) {}
  void onEntry() override { mTimer.start(); }
  std::unique_ptr<State> onDuring() override {
    if (mTimer.getElapsedMilliseconds() > mWaitingTimeMs) {
      terminateMachine();
      return nullptr;
    }
    return nullptr;
  }

 private:
  Timer mTimer;
  int mWaitingTimeMs{3000};
};

class CalculatingPositionState : public State {
 public:
  CalculatingPositionState(GridActor& user) : mUser(user) {}
  std::unique_ptr<State> onDuring() override {
    int x = Helpers::randomInt(-10, 10);
    int y = Helpers::randomInt(-10, 10);
    int z = Helpers::randomInt(-10, 10);
    Vector3DInt newPos = Vector3DInt{x, y, z};
    const GridMap& map = GridMap::getActiveMap();
    if (map.isPosInMap(newPos) && map.isPosFree(newPos)) {
      bool success = map.getLowestPassablePositionFrom(newPos, newPos);
      if (success) {
        return std::make_unique<WalkingState>(mUser, newPos);
      }
    }
    return std::make_unique<WaitingState>(200);
  }

 private:
  GridActor& mUser;
};

// WaitingState::WaitingState(int durationMs) : mWaitingTimeMs(durationMs) {}
// void WaitingState::onEntry() { mTimer.start(); }

// std::unique_ptr<State> WaitingState::onDuring() {
//   if (mTimer.getElapsedMilliseconds() > mWaitingTimeMs) {
//     terminateMachine();
//     return nullptr;
//   }
//   return nullptr;
// }

// CalculatingPositionState::CalculatingPositionState(GridActor& user)
//     : mUser(user) {}

// std::unique_ptr<State> CalculatingPositionState::onDuring() {
//   int x = Helpers::randomInt(-10, 10);
//   int y = Helpers::randomInt(-10, 10);
//   int z = Helpers::randomInt(-10, 10);
//   Vector3DInt newPos = mUser.getPosition() + Vector3DInt{x, y, z};
//   const GridMap& map = GridMap::getActiveMap();
//   if (map.isPosInMap(newPos) && map.isPosFree(newPos)) {
//     bool success = map.getLowestPassablePositionFrom(newPos, newPos);
//     if (success) {
//       return std::make_unique<WalkingState>(mUser, newPos);
//     }
//   }
//   return std::make_unique<WaitingState>(200);
// }

}  // namespace

WalkRandomlyJob::WalkRandomlyJob(GridActor& user)
    : mStateMachine(std::make_unique<CalculatingPositionState>(user)) {}

bool WalkRandomlyJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}

#endif