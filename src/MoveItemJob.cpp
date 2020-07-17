#include "MoveItemJob.h"

#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "ItemContainer.h"
#include "ItemPool.h"
#include "Logging.h"
#include "MoveItemRequestPool.h"
#include "Serializer.h"
#include "WalkingState.h"
namespace {
enum StateType { State_FetchWalkingState, State_PlaceWalkingState };
constexpr int msPerMove = 100;
class FetchWalkingState : public WalkingState {
 public:
  FetchWalkingState(GridActor& user, std::shared_ptr<MoveItemRequest> request)
      : WalkingState(user, msPerMove), mUser(user), mRequest(request) {
    bool success = ItemPool::whereIsClosestItem(
        mUser.getCurrentPos(), mTargetPos, mRequest->getType());
    if (!success) {
      onPathFindFail();
      return;
    }
    success = GridMapHelpers::getClosestFreePositionTo(
        GridMap::getActiveMap(), mTargetPos, mTargetPos, 1);
    if (!success) onPathFindFail();
  }

  FetchWalkingState(GridActor& user, const SerializedObj& serObj)
      : WalkingState(user, serObj),
        mUser(user),
        mRequest(serObj.at("request")),
        mTargetPos(serObj.at("targetPos")) {}

  SerializedObj serialize() const override {
    SerializedObj out;
    out["parent"] = WalkingState::serialize();
    out["type"] = State_FetchWalkingState;
    out["request"] = *mRequest;
    out["targetPos"] = mTargetPos;
    return out;
  }

  Vector3DInt getTargetPos() override { return mTargetPos; }

  std::unique_ptr<State> onReachedTarget() override;
  void onPathFindFail() override {
    MoveItemRequestPool::getInstance().returnRequest(std::move(mRequest));
    terminateMachine();
  }

 private:
  GridActor& mUser;
  std::shared_ptr<MoveItemRequest> mRequest;
  Vector3DInt mTargetPos;
};

class PlaceWalkingState : public WalkingState {
 public:
  PlaceWalkingState(GridActor& user, std::shared_ptr<MoveItemRequest> request,
                    std::unique_ptr<Item>&& item)
      : WalkingState(user, msPerMove),
        mUser(user),
        mRequest(request),
        mItem(std::move(item)) {
    bool success = GridMapHelpers::getClosestFreePositionTo(
        GridMap::getActiveMap(), mRequest->getPos(), mTargetPos, 1, 1);
    if (!success) onPathFindFail();
  }

  PlaceWalkingState(GridActor& user, const SerializedObj& serObj)
      : WalkingState(user, 100),
        mUser(user),
        mRequest(std::make_shared<MoveItemRequest>(serObj.at("request"))),
        mItem(generateItem(serObj.at("item"))),
        mTargetPos(serObj.at("position")) {}

  SerializedObj serialize() const override {
    SerializedObj out;
    out["parent"] = WalkingState::serialize();
    out["type"] = State_PlaceWalkingState;
    out["request"] = *mRequest;
    out["position"] = mTargetPos;
    out["item"] = mItem->getItemType();
    return out;
  }

  Vector3DInt getTargetPos() override { return mTargetPos; }

  std::unique_ptr<State> onReachedTarget() override;
  void onPathFindFail() override {
    GridMap::getActiveMap().addItemAt(mUser.getCurrentPos(), std::move(mItem));
    MoveItemRequestPool::getInstance().returnRequest(std::move(mRequest));
    terminateMachine();
  }

 private:
  GridActor& mUser;
  std::shared_ptr<MoveItemRequest> mRequest;
  std::unique_ptr<Item> mItem;
  Vector3DInt mTargetPos;
};

std::unique_ptr<State> FetchWalkingState::onReachedTarget() {
  const auto& gridActors =
      GridMap::getActiveMap().getGridActorsAt(mUser.getCurrentPos());
  for (const auto& actor : gridActors) {
    if (actor->getType() == GridActor::item) {
      ItemContainer* cmp = actor->owner().getComponent<ItemContainer>();
      ASSERT(cmp, "Got item component without itemcontainer");
      std::unique_ptr<Item> item = cmp->getItem(mRequest->getType());
      if (item) {
        return transitTo<PlaceWalkingState>(mUser, std::move(mRequest),
                                            std::move(item));
      }
    }
  }
  onPathFindFail();
  return noTransition();
}

std::unique_ptr<State> PlaceWalkingState::onReachedTarget() {
  GridMap::getActiveMap().addItemAt(mRequest->getPos(), std::move(mItem));
  terminateMachine();
  return noTransition();
}

std::unique_ptr<State> unserializeState(GridActor& user,
                                        const SerializedObj& serObj) {
  SerializedObj state = serObj["activeState"];
  StateType type = state["type"];
  switch (type) {
    case State_FetchWalkingState:
      return std::make_unique<FetchWalkingState>(user, serObj);
    case State_PlaceWalkingState:
      return std::make_unique<PlaceWalkingState>(user, serObj);
    default:
      ASSERT(false, "Unknown state type");
      return nullptr;
  }
}

}  // namespace

MoveItemJob::MoveItemJob(GridActor& user,
                         std::shared_ptr<MoveItemRequest> request)
    : mStateMachine(std::make_unique<FetchWalkingState>(user, request)) {}

bool MoveItemJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}

MoveItemJob::MoveItemJob(GridActor& user, const SerializedObj& serObj)
    : mStateMachine(unserializeState(user, serObj)) {}

SerializedObj MoveItemJob::serialize() const {
  SerializedObj out;
  out[SerializeString_Type] = IJob::MoveItemJob;
  out["activeState"] = mStateMachine.serializeActiveState();
  return out;
}