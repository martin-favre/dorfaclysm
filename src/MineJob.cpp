#include "MineJob.h"

#include "Block.h"
#include "DorfWalker.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "MiningRequestPool.h"
#include "WalkingState.h"

enum StateType { State_MineWalkingState, State_MiningState };

class MineWalkingState : public WalkingState {
 public:
  MineWalkingState(GridActor& user, std::shared_ptr<MiningRequest> request)
      : WalkingState(user, 100), mRequest(request) {}
  MineWalkingState(GridActor& user, const SerializedObj& serObj)
      : WalkingState(user, serObj.at("parent")),
        mRequest(std::make_shared<MiningRequest>(serObj.at("request"))) {}

  Vector3DInt getTargetPos() override { return mRequest->getPos(); }
  void onPathFindFail() override {
    MiningRequestPool::getInstance().returnRequest(std::move(mRequest));
    terminateMachine();
  }
  std::unique_ptr<State> onReachedTarget() override;

  SerializedObj serialize() const override {
    SerializedObj out;
    out["parent"] = WalkingState::serialize();
    out["type"] = State_MineWalkingState;
    out["request"] = *mRequest;
    return out;
  }

 private:
  std::shared_ptr<MiningRequest> mRequest;
};

class MiningState : public State {
 public:
  MiningState(std::shared_ptr<MiningRequest> request) : mRequest(request) {}
  MiningState(const SerializedObj& serObj)
      : mRequest(std::make_shared<MiningRequest>(serObj.at("request"))) {}
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

std::unique_ptr<State> unserializeState(GridActor& user,
                                        const SerializedObj& serObj) {
  SerializedObj state = serObj["activeState"];
  StateType type = state["type"];
  switch (type) {
    case State_MineWalkingState:
      return std::make_unique<MineWalkingState>(user, serObj);
    case State_MiningState:
      return std::make_unique<MiningState>(serObj);
    default:
      ASSERT(false, "Unknown state type");
      return nullptr;
  }
}

MineJob::MineJob(GridActor& user, const SerializedObj& serObj)
    : mStateMachine(unserializeState(user, serObj)) {}

MineJob::MineJob(GridActor& user, std::shared_ptr<MiningRequest> request)
    : mStateMachine(
          std::make_unique<MineWalkingState>(user, std::move(request))) {}

bool MineJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}

SerializedObj MineJob::serialize() const {
  SerializedObj out;
  out[SerializeString_Type] = IJob::MineJob;
  out["activeState"] = mStateMachine.serializeActiveState();
  return out;
}
