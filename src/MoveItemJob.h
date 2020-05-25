#pragma once
#include "IJob.h"
#include "Serializer.h"
#include "StateMachine.h"

class GridActor;
class MoveItemRequest;
class MoveItemJob : public IJob {
 public:
  MoveItemJob(GridActor& user, std::shared_ptr<MoveItemRequest> request);
  MoveItemJob(GridActor& user, const SerializedObj& serObj);
  bool work() override;
  SerializedObj serialize() const override;

 private:
  StateMachine mStateMachine;
};