#pragma once
#include "DorfWalker.h"
#include "IJob.h"
#include "StateMachine.h"
#include "Timer.h"

class WalkRandomlyJob : public IJob {
 public:
  WalkRandomlyJob(GridActor& user);
  bool work() override;

 private:
  StateMachine mStateMachine;
};