#pragma once

#include <memory>

#include "Component.h"
#include "GridActor.h"
#include "IJob.h"
#include "Vector3DInt.h"

class DorfController : public Component {
 public:
  DorfController(GameObject& gObj);
  DorfController(GameObject& gObj, const SerializedObj& serObj);
  SerializedObj serialize() const override;
  void setup() override;
  void update() override;
  static std::string getTypeString();
 private:
  std::unique_ptr<IJob> unserializeJob(const SerializedObj& serObj);
  void getNewJob();
  GridActor* mGridActor{};
  std::unique_ptr<IJob> mJob;
  std::unique_ptr<SerializedObj> mUnserializedJob;
};