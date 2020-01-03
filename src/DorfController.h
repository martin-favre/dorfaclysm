#pragma once

#include <memory>

#include "Component.h"
#include "IJob.h"
#include "Vector2DInt.h"

class WorldItemComponent;
class DorfController : public Component {
 public:
  DorfController(GameObject& gObj);
  void setup() override;
  void update() override;

 private:
  void getNewJob();
  WorldItemComponent* mWorldItemComp{nullptr};
  Vector2DInt mPreviousPosition;
  std::unique_ptr<IJob> mJob;
};