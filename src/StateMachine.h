#pragma once
#include <memory>

#include "Helpers.h"

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

template <class T, class... Args>
inline std::unique_ptr<T> transitTo(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

inline std::unique_ptr<State> noTransition() { return nullptr; }

class StateMachine {
 public:
  StateMachine(std::unique_ptr<State>&& initialState)
      : mActiveState(std::move(initialState)) {
    ASSERT(mActiveState != nullptr, "Received null first state");
    if (!mActiveState->machineTerminated()) {
      mActiveState->onEntry();
    }
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
        if (!mActiveState->machineTerminated()) {
          mActiveState->onEntry();
        }
      }
    }
  }

 private:
  std::unique_ptr<State> mActiveState;
};
