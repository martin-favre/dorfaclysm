
#include "WalkRandomlyJob.h"

#include <memory>

#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
#include "WalkingState.h"
namespace {

enum StateType { State_WalkingState, State_WaitingState };

class WalkingRandomlyState : public WalkingState {
 public:
  WalkingRandomlyState(GridActor& user, const Vector3DInt& targetPos)
      : WalkingState(user, 500), mTargetPos(targetPos) {}
  Vector3DInt getTargetPos() override { return mTargetPos; }

  std::unique_ptr<State> onReachedTarget() override;

  void onPathFindFail() override { terminateMachine(); }

 private:
  Vector3DInt mTargetPos;
};

class WaitingState : public State {
 public:
  WaitingState(int durationMs) : mWaitingTimeMs(durationMs) {}
  void onEntry() override { mTimer.start(); }
  std::unique_ptr<State> onDuring() override;

 private:
  Timer mTimer;
  int mWaitingTimeMs;
};

class CalculatingPositionState : public State {
 public:
  CalculatingPositionState(GridActor& user) : mUser(user) {}
  std::unique_ptr<State> onDuring() override;

 private:
  GridActor& mUser;
};


std::unique_ptr<State> WalkingRandomlyState::onReachedTarget() {
  return transitTo<WaitingState>(200);
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
      return transitTo<WalkingRandomlyState>(mUser, newPos);
    }
  }
  return transitTo<WaitingState>(200);
}

std::unique_ptr<State> unserializeState(GridActor& user,
                                        const SerializedObj& serObj) {
  SerializedObj state = serObj["activeState"];
  StateType type = state["type"];
  switch (type) {
    case State_WalkingState:
      return std::make_unique<WalkingRandomlyState>(user, serObj);
    case State_WaitingState:
      return std::make_unique<WaitingState>(serObj);
    default:
      ASSERT(false, "Unknown state type");
      return nullptr;
  }
}

}  // namespace

WalkRandomlyJob::WalkRandomlyJob(GridActor& user, const SerializedObj& serObj)
    : mStateMachine(unserializeState(user, serObj)) {}

WalkRandomlyJob::WalkRandomlyJob(GridActor& user)
    : mStateMachine(std::make_unique<CalculatingPositionState>(user)) {}

bool WalkRandomlyJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}

SerializedObj WalkRandomlyJob::serialize() const {
  SerializedObj out;
  out[SerializeString_Type] = IJob::MineJob;
  out["activeState"] = mStateMachine.serializeActiveState();
  return out;
}
