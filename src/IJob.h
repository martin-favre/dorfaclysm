#pragma once
#include "Serializer.h"
class GameObject;
class IJob {
 public:
  enum Type {
    MineJob,
    MoveItemJob,
    WalkRandomlyJob,
    BuildBlockJob
  };
  virtual ~IJob() = default;
  virtual bool work() = 0;
  virtual SerializedObj serialize() const = 0;
  static const std::string SerializeString_Type;
};