#include "MineJob.h"

#include "Block.h"
#include "GameObject.h"
#include "GridMap.h"

class WalkingState : public State {
 public:
  WalkingState(GameObject& user, const Vector3DInt& pos);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  DorfWalker mWalker{200};
  GameObject& mUser;
  Vector3DInt mPos;
};

class MiningState : public State {
 public:
  MiningState(const Vector3DInt& pos) : mPos(pos) {}
  std::unique_ptr<State> onDuring() override;

 private:
  Vector3DInt mPos;
};

std::unique_ptr<State> MiningState::onDuring() {
  GridMap::getActiveMap().removeBlockAt(mPos);
  terminateMachine();
  return nullptr;
}

WalkingState::WalkingState(GameObject& user, const Vector3DInt& pos)
    : mUser(user), mPos(pos) {}

void WalkingState::onEntry() {
  GridMap& map = GridMap::getActiveMap();
  bool success = map.getClosestFreePositionTo(mPos, mPos);
  success = success && mWalker.generateNewPath(mUser.getPosition(), mPos);
  if (!success) {
    map.getBlockAt(mPos).unassignJob();
    terminateMachine();
  }
}
std::unique_ptr<State> WalkingState::onDuring() {
  if (mWalker.isDone()) {
    return std::make_unique<MiningState>(mPos);
  } else {
    Vector3DInt pos = mUser.getPosition();
    mWalker.walkUpdate(pos);
    mUser.setPosition(pos);
    return nullptr;
  }
}

MineJob::MineJob(GameObject& user, const Vector3DInt& targetPos)
    : mDorf(user),
      mStateMachine(
          std::move(std::make_unique<WalkingState>(user, targetPos))) {}

bool MineJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}