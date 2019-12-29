#pragma once

#include <memory>

#include "Component.h"
#include "IJob.h"
class DorfController : public Component {
 public:
  DorfController(GameObject& gObj);
  void update() override;

 private:
  void getNewJob();
  std::unique_ptr<IJob> mJob;
};