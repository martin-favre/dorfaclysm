#pragma once
#include "IJob.h"
#include "StateMachine.h"

class BlockBuildingRequest;
class GridActor;
class MiningRequest;
class BlockBuildingJob : public IJob {
 public:
  BlockBuildingJob(GridActor& user, std::shared_ptr<BlockBuildingRequest>&& request);
  bool work() override;

 private:
  StateMachine mStateMachine;
};