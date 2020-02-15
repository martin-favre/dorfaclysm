#include "MineJob.h"

#include "Block.h"
#include "DorfWalker.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "MiningRequestPool.h"
#include "WalkingState.h"
class MineWalkingState : public WalkingState {
 public:
  MineWalkingState(GridActor& user, std::shared_ptr<MiningRequest> request)
      : WalkingState(user, 100), mRequest(request) {}
  Vector3DInt getTargetPos() override { return mRequest->getPos(); }
  void onPathFindFail() override {
    MiningRequestPool::getInstance().returnRequest(std::move(mRequest));
    terminateMachine();
  }
  std::unique_ptr<State> onReachedTarget() override;

 private:
  std::shared_ptr<MiningRequest> mRequest;
};

class MiningState : public State {
 public:
  MiningState(std::shared_ptr<MiningRequest> request) : mRequest(request) {}
  std::unique_ptr<State> onDuring() override;

 private:
  std::shared_ptr<MiningRequest> mRequest;
};

std::unique_ptr<State> MineWalkingState::onReachedTarget() {
  return transitTo<MiningState>(std::move(mRequest));
}

std::unique_ptr<State> MiningState::onDuring() {
  if (mRequest->isValid()) {
    GridMap::getActiveMap().removeBlockAt(mRequest->getPos());
  }
  terminateMachine();
  return noTransition();
}

MineJob::MineJob(GridActor& user, std::shared_ptr<MiningRequest> request)
    : mStateMachine(
          std::make_unique<MineWalkingState>(user, std::move(request))) {}

bool MineJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}