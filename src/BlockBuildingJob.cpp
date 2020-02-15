#include "BlockBuildingJob.h"

#include "Block.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "BlockBuildingRequestPool.h"
#include "DorfWalker.h"

class WalkingState : public State {
 public:
  WalkingState(GridActor& user, std::shared_ptr<BlockBuildingRequest>&& request)
      : mRequest(std::move(request)), mWalker(user, 100) {}
  void onEntry() override {
    GridMap& map = GridMap::getActiveMap();
    Vector3DInt movePos;
    bool success = GridMapHelpers::getClosestFreePositionTo(
        map, mRequest->getPos(), movePos);
    if (success) {
      mWalker.startNewPath(movePos);
    } else {
      BlockBuildingRequestPool::getInstance().returnRequest(std::move(mRequest));
      terminateMachine();
    }
  }
  std::unique_ptr<State> onDuring() override;

 private:
  std::shared_ptr<BlockBuildingRequest> mRequest;
  DorfWalker mWalker;
};

class BlockBuildingState : public State {
 public:
  BlockBuildingState(std::shared_ptr<BlockBuildingRequest>&& request) : mRequest(request) {}
  std::unique_ptr<State> onDuring() override;

 private:
  std::shared_ptr<BlockBuildingRequest> mRequest;
};

std::unique_ptr<State> BlockBuildingState::onDuring() {
  if (mRequest->isValid()) {
    // GridMap::getActiveMap().setBlockAt(mRequest->getPos());
  }
  terminateMachine();
  return noTransition();
}

std::unique_ptr<State> WalkingState::onDuring() {
  mWalker.update();
  if (mWalker.isIdle()) {
    if (!mWalker.hasFailed()) {
      return transitTo<BlockBuildingState>(std::move(mRequest));
    } else {
      BlockBuildingRequestPool::getInstance().returnRequest(std::move(mRequest));
      terminateMachine();
      return noTransition();
    }
  }
  return noTransition();
}

BlockBuildingJob::BlockBuildingJob(GridActor& user, std::shared_ptr<BlockBuildingRequest>&& request)
    : mStateMachine(std::make_unique<WalkingState>(
          user, std::forward<std::shared_ptr<BlockBuildingRequest>>(request))) {}

bool BlockBuildingJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}