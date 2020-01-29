#pragma once
#include <memory>
class StateMachine;

class State {
 public:
  virtual void onEntry(){};
  virtual ~State() = default;
  /*
    returns ptr to next state when done.
    returns nullptr if not yet done.
  */
  virtual std::unique_ptr<State> onDuring() = 0;
  virtual void onExit(){};
  bool machineTerminated() { return mMachineTerminated; }

 protected:
  void terminateMachine() { mMachineTerminated = true; }

 private:
  bool mMachineTerminated{false};
};

class StateMachine {
 public:
  StateMachine(std::unique_ptr<State>&& initialState)
      : mActiveState(std::move(initialState)) {
    ASSERT(mActiveState != nullptr, "Received null first state");
    mActiveState->onEntry();
  }

  bool isTerminated() {
    ASSERT(mActiveState != nullptr, "mActiveState is null");
    return mActiveState->machineTerminated();
  }

  void update() {
    ASSERT(mActiveState != nullptr, "Received null first state");
    if (!mActiveState->machineTerminated()) {
      std::unique_ptr<State> nextState = mActiveState->onDuring();
      if (nextState != nullptr) {
        mActiveState->onExit();
        mActiveState = std::move(nextState);
        mActiveState->onEntry();
      }
    }
  }

 private:
  std::unique_ptr<State> mActiveState;
};
