#pragma once
#include <algorithm>
#include <memory>

#include "JobPool.h"
#include "Sprite.h"

class WorldItem;
class Block {
 public:
  virtual const Sprite* getSprite() const = 0;
  virtual bool isClickable() const = 0;
  virtual bool mayPassThrough() const = 0;
  virtual bool mayClimbUpFrom() const = 0;
  virtual bool mayWalkOnTop() const = 0;
  virtual const std::string& getName() const = 0;
  virtual bool supportsJob(JobType type) const = 0;
  virtual bool isSeeThrough() const = 0;

  void assignJob(JobType type);
  void unassignJob();
  JobType getJob() const;

 private:
  JobType mAssignedJob{jobTypeUnset};
};