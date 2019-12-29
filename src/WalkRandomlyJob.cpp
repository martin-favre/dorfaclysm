#include "WalkRandomlyJob.h"

#include <memory>

#include "GameObject.h"
#include "GridMap.h"
#include "Helpers.h"
namespace {
class WalkingState;
class WaitingState;

class WalkingState : public State {
 public:
  WalkingState(GameObject& user);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  DorfWalker mWalker;
  GameObject& mUser;
};

class WaitingState : public State {
 public:
  WaitingState(GameObject& user);
  void onEntry() override;
  std::unique_ptr<State> onDuring() override;

 private:
  GameObject& mUser;
  Timer mTimer;
  int mWaitingTimeMs{3000};
};

WalkingState::WalkingState(GameObject& user) : mUser(user) {}
void WalkingState::onEntry() {
  int x = Helpers::randomInt(0, 20);
  int y = Helpers::randomInt(0, 20);
  mWalker.generateNewPath(mUser.getPosition(), {x, y});
}
std::unique_ptr<State> WalkingState::onDuring() {
  if (mWalker.isDone()) {
    return std::make_unique<WaitingState>(mUser);
  } else {
    Vector2DInt pos = mUser.getPosition();
    mWalker.walkUpdate(pos);
    mUser.setPosition(pos);
    return nullptr;
  }
}

WaitingState::WaitingState(GameObject& user) : mUser(user) {}
void WaitingState::onEntry() { mTimer.start(); }

std::unique_ptr<State> WaitingState::onDuring() {
  if (mTimer.getElapsedMilliseconds() > mWaitingTimeMs) {
    terminateMachine();
    return nullptr;
  }
  return nullptr;
}

}  // namespace

WalkRandomlyJob::WalkRandomlyJob(GameObject& user)
    : mDorf(user),
      mStateMachine(std::move(std::make_unique<WalkingState>(user))) {}

bool WalkRandomlyJob::work() { return mStateMachine.update(); }