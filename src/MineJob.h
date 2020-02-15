#pragma once
#include "IJob.h"
#include "StateMachine.h"

class GridActor;
class MiningRequest;
class MineJob : public IJob {
 public:
  MineJob(GridActor& user, std::shared_ptr<MiningRequest> request);
  bool work() override;

 private:
  StateMachine mStateMachine;
};