#pragma once
#include "IJob.h"
#include "StateMachine.h"

class GridActor;
class MoveItemRequest;
class SerializedObj;
class MoveItemJob : public IJob {
 public:
  MoveItemJob(GridActor& user, std::shared_ptr<MoveItemRequest> request);
  MoveItemJob(GridActor& user, const SerializedObj& serObj);
  bool work() override;

 private:
  StateMachine mStateMachine;
};