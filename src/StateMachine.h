#pragma once
#include <memory>

#include "Helpers.h"
#include "Serializer.h"
class StateMachine;

class State {
 public:
  State() = default;
  State(const SerializedObj& serObj)
      : mMachineTerminated(serObj.at("terminated")) {}
  virtual void onEntry(){};
  virtual ~State() = default;
  /*
    returns ptr to next state when done.
    returns nullptr if not yet done.
  */
  virtual std::unique_ptr<State> onDuring() = 0;
  virtual void onExit(){};
  bool isMachineTerminated() { return mMachineTerminated; }

  virtual SerializedObj serialize() const {
    SerializedObj obj;
    obj["terminated"] = mMachineTerminated;
    return obj;
  }

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
    if (!mActiveState->isMachineTerminated()) {
      mActiveState->onEntry();
    }
  }

  bool isTerminated() {
    ASSERT(mActiveState != nullptr, "mActiveState is null");
    return mActiveState->isMachineTerminated();
  }

  void update() {
    ASSERT(mActiveState != nullptr, "Received null first state");
    if (!mActiveState->isMachineTerminated()) {
      std::unique_ptr<State> nextState = mActiveState->onDuring();
      if (nextState != nullptr) {
        mActiveState->onExit();
        mActiveState = std::move(nextState);
        if (!mActiveState->isMachineTerminated()) {
          mActiveState->onEntry();
        }
      }
    }
  }

  SerializedObj serializeActiveState() const {
    return mActiveState->serialize();
  }
  
  friend void to_json(SerializedObj& out, const StateMachine& machine);

 private:
  std::unique_ptr<State> mActiveState;
};