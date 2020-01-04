#include "MineJob.h"

#include "GameObject.h"
#include "GridMap.h"
#include "Tile.h"

class WalkingState : public State {
 public:
  WalkingState(GameObject& user, const Vector2DInt& pos);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  DorfWalker mWalker{200};
  GameObject& mUser;
  Vector2DInt mPos;
};

class MiningState : public State {
 public:
  MiningState(const Vector2DInt& pos) : mPos(pos) {}
  std::unique_ptr<State> onDuring() override;

 private:
  Vector2DInt mPos;
};

std::unique_ptr<State> MiningState::onDuring() {
  WorldTile& wTile = GridMap::getActiveMap().getWorldTile(mPos);
  wTile.removeTile();
  terminateMachine();
  return nullptr;
}

WalkingState::WalkingState(GameObject& user, const Vector2DInt& pos)
    : mUser(user), mPos(pos) {}

void WalkingState::onEntry() {
  bool success = mWalker.generateNewPath(mUser.getPosition(), mPos);
  if(!success)
  {
    Tile* tile = GridMap::getActiveMap().getWorldTile(mPos).getTile();
    if(tile)
    {
      tile->unAssignJob();
    }
    terminateMachine();
  }
}
std::unique_ptr<State> WalkingState::onDuring() {
  if (mWalker.isDone()) {
    return std::make_unique<MiningState>(mPos);
  } else {
    Vector2DInt pos = mUser.getPosition();
    mWalker.walkUpdate(pos);
    mUser.setPosition(pos);
    return nullptr;
  }
}

MineJob::MineJob(GameObject& user, const Vector2DInt& targetPos)
    : mDorf(user),
      mStateMachine(
          std::move(std::make_unique<WalkingState>(user, targetPos))) {}

bool MineJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}