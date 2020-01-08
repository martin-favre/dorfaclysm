#pragma once

#include <memory>

#include "Component.h"
#include "IJob.h"
#include "Vector3DInt.h"
#include "WorldItem.h"

class DorfController : public Component {
 public:
  DorfController(GameObject& gObj);
  void setup() override;
  void update() override;
  void teardown() override;
 private:
  void getNewJob();
  WorldItem mWorldItem{"Dorf"};
  std::unique_ptr<IJob> mJob;
};