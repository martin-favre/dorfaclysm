#include "BlockBuildingJob.h"

#include "Block.h"
#include "BlockBuildingRequestPool.h"
#include "DorfWalker.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "WalkingState.h"
namespace {

enum StateType { State_WalkingToBuildingState, State_BlockBuildingState };

class WalkingToBuildingState : public WalkingState {
 public:
  WalkingToBuildingState(GridActor& user,
                         std::shared_ptr<BlockBuildingRequest>&& request)
      : WalkingState(user, 100), mRequest(std::move(request)) {
    bool success = GridMapHelpers::getClosestFreePositionTo(
        GridMap::getActiveMap(), mRequest->getPos(), mTargetPos);
    if (!success) {
      onPathFindFail();
    }
  }

  WalkingToBuildingState(GridActor& user, const SerializedObj& serObj)
      : WalkingState(user, serObj.at("parent")),
        mRequest(std::make_shared<BlockBuildingRequest>(serObj.at("request"))),
        mTargetPos(serObj.at("targetPos")) {}

  SerializedObj serialize() const override {
    SerializedObj out;
    out["parent"] = WalkingState::serialize();
    out["type"] = State_WalkingToBuildingState;
    out["request"] = *mRequest;
    out["targetPos"] = mTargetPos;
    return out;
  }

 protected:
  Vector3DInt getTargetPos() override { return mTargetPos; }
  std::unique_ptr<State> onReachedTarget() override;
  void onPathFindFail() override {
    BlockBuildingRequestPool::getInstance().returnRequest(std::move(mRequest));
    terminateMachine();
  }

 private:
  std::shared_ptr<BlockBuildingRequest> mRequest;
  Vector3DInt mTargetPos;
};

class BlockBuildingState : public State {
 public:
  BlockBuildingState(std::shared_ptr<BlockBuildingRequest>&& request)
      : mRequest(request) {}

  BlockBuildingState(const SerializedObj& serObj)
      : mRequest(std::make_shared<BlockBuildingRequest>(serObj.at("request"))) {
  }

  std::unique_ptr<State> onDuring() override;
  SerializedObj serialize() const override {
    SerializedObj out;
    out["type"] = State_BlockBuildingState;
    out["request"] = *mRequest;
    return out;
  }

 private:
  std::shared_ptr<BlockBuildingRequest> mRequest;
};

std::unique_ptr<State> BlockBuildingState::onDuring() {
  if (mRequest->isValid()) {
    // GridMap::getActiveMap().setBlockAt(mRequest->getPos());
    // Currently the block checks the location itself
  }
  terminateMachine();
  return noTransition();
}

std::unique_ptr<State> WalkingToBuildingState::onReachedTarget() {
  return transitTo<BlockBuildingState>(std::move(mRequest));
}

std::unique_ptr<State> unserializeState(GridActor& user,
                                        const SerializedObj& serObj) {
  SerializedObj state = serObj["activeState"];
  StateType type = state["type"];
  switch (type) {
    case State_WalkingToBuildingState:
      return std::make_unique<WalkingToBuildingState>(user, serObj);
    case State_BlockBuildingState:
      return std::make_unique<BlockBuildingState>(serObj);
    default:
      ASSERT(false, "Unknown state type");
      return nullptr;
  }
}
}  // namespace

BlockBuildingJob::BlockBuildingJob(
    GridActor& user, std::shared_ptr<BlockBuildingRequest>&& request)
    : mStateMachine(std::make_unique<WalkingToBuildingState>(
          user, std::forward<std::shared_ptr<BlockBuildingRequest>>(request))) {
}

BlockBuildingJob::BlockBuildingJob(GridActor& user, SerializedObj& serObj)
    : mStateMachine(unserializeState(user, serObj))

{}

bool BlockBuildingJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}

SerializedObj BlockBuildingJob::serialize() const {
  SerializedObj out;
  out[SerializeString_Type] = IJob::MoveItemJob;
  out["activeState"] = mStateMachine.serializeActiveState();
  return out;
}