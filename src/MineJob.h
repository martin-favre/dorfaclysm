#pragma once
#include "DorfWalker.h"
#include "IJob.h"
#include "StateMachine.h"
#include "Timer.h"

class MineJob : public IJob {
 public:
  MineJob(GameObject& user, const Vector3DInt& targetPos);
  bool work() override;

 private:
  GameObject& mDorf;
  StateMachine mStateMachine;
};