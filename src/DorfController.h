#pragma once

#include <memory>

#include "Component.h"
#include "GridActor.h"
#include "IJob.h"
#include "Vector3DInt.h"

class DorfController : public Component {
 public:
  DorfController(GameObject& gObj);
  void setup() override;
  void update() override;

 private:
  void getNewJob();
  GridActor* mGridActor{};
  std::unique_ptr<IJob> mJob;
};