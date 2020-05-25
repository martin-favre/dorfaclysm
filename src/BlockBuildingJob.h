#pragma once
#include "IJob.h"
#include "StateMachine.h"

class BlockBuildingRequest;
class GridActor;
class MiningRequest;
class BlockBuildingJob : public IJob {
 public:
  BlockBuildingJob(GridActor& user, std::shared_ptr<BlockBuildingRequest>&& request);
  BlockBuildingJob(GridActor& user, SerializedObj& serObj);
  bool work() override;
  SerializedObj serialize() const override;
 private:
  StateMachine mStateMachine;
};