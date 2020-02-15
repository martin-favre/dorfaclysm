#include "MoveItemJob.h"

#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "ItemContainer.h"
#include "ItemPool.h"
#include "Logging.h"
#include "MoveItemRequestPool.h"
#include "WalkingState.h"
class FetchWalkingState : public WalkingState {
 public:
  FetchWalkingState(GridActor& user, std::shared_ptr<MoveItemRequest> request)
      : WalkingState(user, 100), mUser(user), mRequest(request) {
    bool success = ItemPool::whereIsClosestItem(
        mUser.getCurrentPos(), mTargetPos, mRequest->getType());
    if (!success) onPathFindFail();
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
      : WalkingState(user, 100),
        mUser(user),
        mRequest(request),
        mItem(std::move(item)) {}
  Vector3DInt getTargetPos() override { return mRequest->getPos(); }

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
  GridMap::getActiveMap().addItemAt(mUser.getCurrentPos(), std::move(mItem));
  terminateMachine();
  return noTransition();
}

MoveItemJob::MoveItemJob(GridActor& user,
                         std::shared_ptr<MoveItemRequest> request)
    : mStateMachine(std::make_unique<FetchWalkingState>(user, request)) {}
bool MoveItemJob::work() {
  mStateMachine.update();
  return mStateMachine.isTerminated();
}
