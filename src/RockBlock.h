#pragma once
#include "Block.h"
class RockBlock : public Block {
 public:
  RockBlock();
  const Sprite* getSprite() const override;
  bool isClickable() const override;
  bool mayPassThrough() const override;
  bool mayClimbUpFrom() const override;
  bool mayWalkOnTop() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;
  bool supportsJob(JobType type) const override;

 private:
  static std::unique_ptr<Sprite> mSprite;
  static const std::string mName;
};