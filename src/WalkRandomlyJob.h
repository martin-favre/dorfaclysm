#pragma once
#include "DorfWalker.h"
#include "IJob.h"
#include "StateMachine.h"
#include "Timer.h"

class WalkRandomlyJob : public IJob {
 public:
  WalkRandomlyJob(GameObject& user);
  bool work() override;

 private:
  GameObject& mDorf;
  StateMachine mStateMachine;
};