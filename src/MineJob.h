#pragma once
#include "DorfWalker.h"
#include "IJob.h"
#include "MiningRequestPool.h"
#include "StateMachine.h"
#include "Timer.h"

class MineJob : public IJob {
 public:
  MineJob(GridActor& user, std::unique_ptr<MiningRequest>&& request);
  bool work() override;

 private:
  std::unique_ptr<MiningRequest> mRequest;
  StateMachine mStateMachine;
};