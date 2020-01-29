#pragma once
class GameObject;
class IJob {
 public:
  virtual ~IJob() = default;
  virtual bool work() = 0;
};