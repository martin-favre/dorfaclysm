#pragma once
class GameObject;
class IJob {
 public:
  virtual bool work() = 0;
};