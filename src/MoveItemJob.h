#pragma once
#include "IJob.h"
#include "StateMachine.h"

class GridActor;
class MoveItemRequest;
class MoveItemJob : public IJob {
 public:
  MoveItemJob(GridActor& user, std::shared_ptr<MoveItemRequest> request);
  bool work() override;

 private:
  StateMachine mStateMachine;
};