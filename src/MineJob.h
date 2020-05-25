#pragma once
#include "GridActor.h"
#include "IJob.h"
#include "Serializer.h"
#include "StateMachine.h"

class MiningRequest;
class MineJob : public IJob {
 public:
  MineJob(GridActor& user, const SerializedObj& serObj);
  MineJob(GridActor& user, std::shared_ptr<MiningRequest> request);
  bool work() override;
  SerializedObj serialize() const override;
 private:
  StateMachine mStateMachine;
};