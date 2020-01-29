#include "MineJob.h"

#include "Block.h"
#include "GameObject.h"
#include "GridMap.h"
#include "GridMapHelpers.h"

class WalkingState : public State {
 public:
  WalkingState(GameObject& user, MiningRequest& request);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  DorfWalker mWalker{200};
  GameObject& mUser;
  MiningRequest& mRequest;
};

class MiningState : public State {
 public:
  MiningState(MiningRequest& request) : mRequest(request) {}
  std::unique_ptr<State> onDuring() override;

 private:
  MiningRequest& mRequest;
};

std::unique_ptr<State> MiningState::onDuring() {
  if (mRequest.isValid()) {
    GridMap::getActiveMap().removeBlockAt(mRequest.getPos());
  }
  terminateMachine();
  return nullptr;
}

WalkingState::WalkingState(GameObject& user, MiningRequest& request)
    : mUser(user), mRequest(request) {}

void WalkingState::onEntry() {
  GridMap& map = GridMap::getActiveMap();
  Vector3DInt movePos;
  bool success =
      GridMapHelpers::getClosestFreePositionTo(map, mRequest.getPos(), movePos);
  success = success && mWalker.generateNewPath(mUser.getPosition(), movePos);
  if (!success) {
    terminateMachine();
  }
}
std::unique_ptr<State> WalkingState::onDuring() {
  if (mWalker.isDone()) {
    return std::make_unique<MiningState>(mRequest);
  } else {
    Vector3DInt pos = mUser.getPosition();
    mWalker.walkUpdate(pos);
    mUser.setPosition(pos);
    return nullptr;
  }
}

MineJob::MineJob(GameObject& user, std::unique_ptr<MiningRequest>&& request)
    : mRequest(std::move(request)),
      mStateMachine(std::make_unique<WalkingState>(user, *mRequest)) {}

bool MineJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}