#pragma once
#include "DorfWalker.h"
#include "IJob.h"
#include "StateMachine.h"
#include "Timer.h"

class WalkRandomlyJob : public IJob {
 public:
  WalkRandomlyJob(GridActor& user, const SerializedObj& serObj);
  WalkRandomlyJob(GridActor& user);
  bool work() override;
  SerializedObj serialize() const override;

 private:
  StateMachine mStateMachine;
};