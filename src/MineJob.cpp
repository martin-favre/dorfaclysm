#include "MineJob.h"

#include "Block.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "MiningRequestPool.h"

class WalkingState : public State {
 public:
  WalkingState(GridActor& user, MiningRequest& request)
      : mRequest(request), mWalker(user, 100) {}
  void onEntry() override {
    GridMap& map = GridMap::getActiveMap();
    Vector3DInt movePos;
    bool success = GridMapHelpers::getClosestFreePositionTo(
        map, mRequest.getPos(), movePos);
    if (success) {
      mWalker.startNewPath(movePos);
    } else {
      terminateMachine();
    }
  }
  std::unique_ptr<State> onDuring() override;

 private:
  MiningRequest& mRequest;
  DorfWalker mWalker;
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
  return noTransition();
}

std::unique_ptr<State> WalkingState::onDuring() {
  mWalker.update();
  if (mWalker.isIdle()) {
    if (!mWalker.hasFailed()) {
      return transitTo<MiningState>(mRequest);
    } else {
      terminateMachine();
      return noTransition();
    }
  }
  return noTransition();
}

MineJob::MineJob(GridActor& user, std::shared_ptr<MiningRequest>&& request)
    : mRequest(std::move(request)),
      mStateMachine(std::make_unique<WalkingState>(user, *mRequest)) {}

bool MineJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}