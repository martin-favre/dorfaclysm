#pragma once

#include "Block.h"

// for unit tests
class DefaultBlock : public Block {
 public:
  const Sprite* getSprite() const override { return nullptr; }
  bool isClickable() const override { return false; }
  bool mayPassThrough() const override { return false; }
  bool mayClimbUpFrom() const override { return false; }
  bool mayWalkOnTop() const override { return false; }
  const std::string& getName() const override { return mName; }
  bool supportsJob(JobType type) const override {
    (void)type;
    return false;
  };
  bool isSeeThrough() const override { return false; };

 private:
  const std::string mName;
};